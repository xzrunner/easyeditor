#ifndef _EASYUI_WINDOW_LIBRARY_PAGE_H_
#define _EASYUI_WINDOW_LIBRARY_PAGE_H_



namespace eui
{
namespace window
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent, const char* name);

	virtual bool IsHandleSymbol(ee::Symbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage 

}
}

#endif // _EASYUI_WINDOW_LIBRARY_PAGE_H_