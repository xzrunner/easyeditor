#pragma once

#include <ee/CombinedDropTarget.h>

#include <node3/SceneNode.h>

namespace ee { class LibraryPanel; }

namespace ee3
{

class StagePanel;
class SceneTreeCtrl;

class StageDropTarget : public ee::CombinedDropTarget
{
public:
	StageDropTarget(wxWindow* stage_wnd, ee::LibraryPanel* library,
		StagePanel* stage);

	virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text) override;
	virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override;

private:
	void InsertNode(const n3::SceneNodePtr& node);

private:
	ee::LibraryPanel* m_library;
	StagePanel*       m_stage;

}; // StageDropTarget

}