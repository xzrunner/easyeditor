#ifndef _EASYICON_LIBRARY_PAGE_H_
#define _EASYICON_LIBRARY_PAGE_H_



namespace eicon
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

#endif // _EASYICON_LIBRARY_PAGE_H_