#pragma once

#include <ee/CombinedDropTarget.h>

namespace ee { class LibraryPanel; }

namespace enode3d
{

class StagePanel;

class StageDropTarget : public ee::CombinedDropTarget
{
public:
	StageDropTarget(wxWindow* stage_wnd, StagePanel* stage, ee::LibraryPanel* library);

	virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text) override;
	virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override;

private:
	StagePanel*       m_stage;
	ee::LibraryPanel* m_library;

}; // StageDropTarget

}