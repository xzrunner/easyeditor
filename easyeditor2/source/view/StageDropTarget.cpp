#include "ee2/StageDropTarget.h"
#include "ee2/StagePanel.h"
#include "ee2/NodeFactory.h"
#include "ee2/StageCanvas.h"

#include <ee/StringHelper.h>
#include <ee/LibraryPanel.h>

#include <ee0/MessageID.h>
#include <ee0/VariantSet.h>

#include <guard/check.h>
#include <node2/CompTransform.h>
#include <sprite2/OrthoCamera.h>

namespace ee2
{

StageDropTarget::StageDropTarget(wxWindow* stage_wnd, ee::LibraryPanel* library,
		                         StagePanel* stage)
	: CombinedDropTarget(stage_wnd)
	, m_library(library)
	, m_stage(stage)
{
}

void StageDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
	std::vector<std::string> keys;
	ee::StringHelper::Split(text.ToStdString(), ",", keys);

	if (keys.size() <= 1) {
		return;
	}

	for (int i = 1, n = keys.size(); i < n; ++i)
	{
		int idx = ee::StringHelper::FromString<int>(keys[i]);
		auto sym = m_library->GetSymbol(idx);
		if (!sym) {
			continue;
		}

		sm::vec2 pos = TransPosScrToProj(x, y);
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
		//auto parent = node->GetParent();
		//if (parent) {
		//	auto p_pos = parent->GetComponent<n3::CompTransform>().GetTransformMat() * sm::vec3(0, 0, 0);
		//	pos -= p_pos;
		//}
		ctrans.SetPosition(pos);
	}

	m_stage->GetSubjectMgr().NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void StageDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
}

void StageDropTarget::InsertNode(const n0::SceneNodePtr& node)
{
	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_PVOID;
	var.m_val.pv = &std::const_pointer_cast<n0::SceneNode>(node);
	vars.SetVariant("node", var);
	
	bool succ = m_stage->GetSubjectMgr().NotifyObservers(ee0::MSG_INSERT_SCENE_NODE, vars);
	GD_ASSERT(succ, "no MSG_INSERT_SCENE_NODE");
}

sm::vec2 StageDropTarget::TransPosScrToProj(int x, int y) const
{
	auto canvas = std::dynamic_pointer_cast<StageCanvas>(m_stage->GetCanvas());
	auto& cam = canvas->GetCamera();
	if (cam->Type() != s2::CAM_ORTHO2D) {
		return sm::vec2(0, 0);
	}
	
	int w = m_stage->GetSize().GetWidth(),
		h = m_stage->GetSize().GetHeight();
	return std::dynamic_pointer_cast<s2::OrthoCamera>(cam)->
		TransPosScreenToProject(x, y, w, h);
}

}