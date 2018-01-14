#include "StageDropTarget.h"
#include "StagePanel.h"
#include "NodeFactory.h"

#include <ee/StringHelper.h>
#include <ee/LibraryPanel.h>

namespace enode3d
{

StageDropTarget::StageDropTarget(wxWindow* stage_wnd, StagePanel* stage, ee::LibraryPanel* library)
	: CombinedDropTarget(stage_wnd)
	, m_stage(stage)
	, m_library(library){
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

		sm::vec3 pos = m_stage->TransPosScrToProj(x, y);
		bool handled = OnDropSymbol(sym, sm::vec2(pos.x, pos.y));
		if (handled) {
			continue;
		}

		auto node = NodeFactory::Instance()->Create(sym);
		if (node) {
			node->SetPos(pos);
			m_stage->InsertNode(node);
		}
	}
}

void StageDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
}

}