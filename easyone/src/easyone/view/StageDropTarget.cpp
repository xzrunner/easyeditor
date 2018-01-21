#include "view/StageDropTarget.h"
#include "view/StagePanel.h"
#include "view/SceneTreeCtrl.h"
#include "data/NodeFactory.h"

#include <ee/StringHelper.h>
#include <ee/LibraryPanel.h>

namespace eone
{

StageDropTarget::StageDropTarget(wxWindow* stage_wnd, ee::LibraryPanel* library,
		                         StagePanel* stage, SceneTreeCtrl* tree)
	: CombinedDropTarget(stage_wnd)
	, m_library(library)
	, m_stage(stage)
	, m_tree(tree)
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

		sm::vec3 pos = m_stage->TransPosScrToProj3d(x, y);
		bool handled = OnDropSymbol(sym, sm::vec2(pos.x, pos.y));
		if (handled) {
			continue;
		}

		auto node = NodeFactory::Instance()->Create(sym);
		if (node) {
			auto& ctrans = node->AddComponent<n3::CompTransform>();
			ctrans.SetPosition(pos);
			InsertNode(node);
		}
	}
}

void StageDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
}

void StageDropTarget::InsertNode(const SceneNodePtr& node)
{
	m_stage->InsertNode(node);
	m_tree->InsertNode(node);
}

}