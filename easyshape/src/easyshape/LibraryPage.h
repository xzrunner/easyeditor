#ifndef _ESHAPE_LIBRARY_PAGE_H_
#define _ESHAPE_LIBRARY_PAGE_H_



namespace eshape
{

class StagePanel;
class ToolbarPanel;
class LibraryItem;

class LibraryPage : public ee::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent, const char* name);

	void clickListSelection();

	void setContext(StagePanel* stage, ToolbarPanel* toolbar);

private:
	virtual void onDelPress(wxCommandEvent& event);

protected:
	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // LibraryPage

}

#endif // _ESHAPE_LIBRARY_PAGE_H_