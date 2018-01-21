#pragma once

#include "data/SceneNode.h"

#include <ee/CombinedDropTarget.h>

namespace ee { class LibraryPanel; }

namespace eone
{

class StagePanel;
class SceneTreeCtrl;

class StageDropTarget : public ee::CombinedDropTarget
{
public:
	StageDropTarget(wxWindow* stage_wnd, ee::LibraryPanel* library,
		StagePanel* stage, SceneTreeCtrl* tree);

	virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text) override;
	virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override;

private:
	void InsertNode(const SceneNodePtr& node);

private:
	ee::LibraryPanel* m_library;

	StagePanel*       m_stage;
	SceneTreeCtrl*    m_tree;

}; // StageDropTarget

}