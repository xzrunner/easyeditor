#ifndef _EASYTEXT_LIBRARY_PAGE_H_
#define _EASYTEXT_LIBRARY_PAGE_H_



namespace etext
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(ee::Symbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _EASYTEXT_LIBRARY_PAGE_H_