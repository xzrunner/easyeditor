#ifndef _EASYUI_TOP_STAGE_PANEL_H_
#define _EASYUI_TOP_STAGE_PANEL_H_



namespace eui
{

class TopPannels;
class UIStagePage;

class TopStagePanel : public wxPanel
{
public:
	TopStagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels);

	UIStagePage* GetSelectedPage();

private:
	void InitLayout();
	void InitTabPages(wxSizer* sizer);

	void OnPageChanging(wxNotebookEvent& event);
	void OnPageChanged(wxNotebookEvent& event);

private:
	wxTopLevelWindow* m_frame;

	TopPannels* m_top_pannels;

	wxNotebook* m_notebook;

	std::vector<UIStagePage*> m_pages;

	DECLARE_EVENT_TABLE()

}; // TopStagePanel

}

#endif // _EASYUI_TOP_STAGE_PANEL_H_
