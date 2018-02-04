#include "ee3/WxStageDropTarget.h"
#include "ee3/WxStagePage.h"
#include "ee3/NodeFactory.h"

#include <ee0/WxLibraryPanel.h>
#include <ee0/MessageID.h>
#include <ee0/VariantSet.h>
#include <ee0/WxLibraryItem.h>

#include <guard/check.h>
#include <node0/SceneNode.h>
#include <node3/CompTransform.h>
#include <gum/StringHelper.h>
#include <gum/SymbolPool.h>

namespace ee3
{

WxStageDropTarget::WxStageDropTarget(ee0::WxLibraryPanel* library, WxStagePage* stage)
	: m_library(library)
	, m_stage(stage)
{
}

void WxStageDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
	CU_VEC<CU_STR> keys;
	gum::StringHelper::Split(text.ToStdString().c_str(), ",", keys);

	if (keys.size() <= 1) {
		return;
	}

	for (int i = 1, n = keys.size(); i < n; ++i)
	{
		int idx = std::stoi(keys[i].c_str());
		auto item = m_library->GetItem(idx);
		if (!item) {
			continue;
		}

		auto sym = gum::SymbolPool::Instance()->Fetch(item->GetFilepath().c_str());

		sm::vec3 pos = m_stage->TransPosScrToProj3d(x, y);
		bool handled = OnDropSymbol(sym, sm::vec2(pos.x, pos.y));
		if (handled) {
			continue;
		}

		auto node = NodeFactory::Instance()->Create(sym);
		if (!node) {
			continue;
		}

		InsertNode(node);

		// transform
		auto& ctrans = node->AddComponent<n3::CompTransform>();
		auto parent = node->GetParent();
		if (parent) {
			auto p_pos = parent->GetComponent<n3::CompTransform>().GetTransformMat() * sm::vec3(0, 0, 0);
			pos -= p_pos;
		}
		ctrans.SetPosition(pos);
	}

	m_stage->GetSubjectMgr().NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxStageDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
}

void WxStageDropTarget::InsertNode(const n0::SceneNodePtr& node)
{
	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_PVOID;
	var.m_val.pv = &std::const_pointer_cast<n0::SceneNode>(node);
	vars.SetVariant("node", var);
	
	bool succ = m_stage->GetSubjectMgr().NotifyObservers(ee0::MSG_INSERT_SCENE_NODE, vars);
	GD_ASSERT(succ, "no MSG_INSERT_SCENE_NODE");
}

}