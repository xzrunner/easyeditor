#include "ee2/WxStageDropTarget.h"
#include "ee2/WxStagePage.h"
#include "ee2/NodeFactory.h"
#include "ee2/WxStageCanvas.h"

#include <ee0/MessageID.h>
#include <ee0/VariantSet.h>
#include <ee0/CameraHelper.h>
#include <ee0/WxDropTarget.h>
#include <ee0/WxLibraryPanel.h>
#include <ee0/WxLibraryItem.h>

#include <guard/check.h>
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>
#include <sprite2/OrthoCamera.h>
#include <gum/StringHelper.h>
#include <gum/SymbolPool.h>

namespace ee2
{

WxStageDropTarget::WxStageDropTarget(ee0::WxLibraryPanel* library, WxStagePage* stage)
	: WxDropTarget()
	, m_library(library)
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

		auto& cam = std::dynamic_pointer_cast<WxStageCanvas>(m_stage->GetImpl().GetCanvas())->GetCamera();
		GD_ASSERT(cam, "null cam");
		sm::vec2 pos = ee0::CameraHelper::TransPosScreenToProject(*cam, x, y);
		bool handled = OnDropSymbol(sym, pos);
		if (handled) {
			continue;
		}

		auto node = NodeFactory::Instance()->Create(sym);
		if (!node) {
			continue;
		}

		InsertNode(node);

		// transform
		auto& ctrans = node->AddComponent<n2::CompTransform>();
		auto parent = node->GetParent();
		if (parent) {
			auto p_pos = parent->GetComponent<n2::CompTransform>().GetTrans().GetMatrix() * sm::vec2(0, 0);
			pos -= p_pos;
		}
		ctrans.GetTrans().SetPosition(pos);

		// bounding box
		auto& bounding = node->GetComponent<n2::CompBoundingBox>();
		bounding.Build(ctrans.GetTrans().GetSRT());
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