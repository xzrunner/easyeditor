#pragma once

#include <ee/CombinedDropTarget.h>

#include <node0/SceneNode.h>

namespace ee { class LibraryPanel; }

namespace ee2
{

class StagePanel;

class StageDropTarget : public ee::CombinedDropTarget
{
public:
	StageDropTarget(wxWindow* stage_wnd, ee::LibraryPanel* library,
		StagePanel* stage);

	virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text) override;
	virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override;

private:
	void InsertNode(const n0::SceneNodePtr& node);

	sm::vec2 TransPosScrToProj(int x, int y) const;

private:
	ee::LibraryPanel* m_library;
	StagePanel*       m_stage;

}; // StageDropTarget

}