#pragma once

#include <wx/aui/auibook.h>

namespace ee0 { class StagePage; }

namespace eone
{

class StagePanel : public wxAuiNotebook
{
public:
	StagePanel(wxWindow* parent);

	ee0::StagePage* GetCurrentStagePage() const;

private:
	void OnPageChanging(wxAuiNotebookEvent& event);
	void OnPageChanged(wxAuiNotebookEvent& event);

private:
	ee0::StagePage* m_last_page;

}; // StagePanel

}