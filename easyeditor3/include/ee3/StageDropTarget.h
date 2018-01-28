#pragma once

#include <ee/CombinedDropTarget.h>

#include <node0/SceneNode.h>

namespace ee { class LibraryPanel; }

namespace ee3
{

class StagePage;

class StageDropTarget : public ee::CombinedDropTarget
{
public:
	StageDropTarget(wxWindow* stage_wnd, ee::LibraryPanel* library,
		StagePage* stage);

	virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text) override;
	virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override;

private:
	void InsertNode(const n0::SceneNodePtr& node);

private:
	ee::LibraryPanel* m_library;
	StagePage*       m_stage;

}; // StageDropTarget

}