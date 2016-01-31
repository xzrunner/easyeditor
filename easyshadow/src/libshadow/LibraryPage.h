#ifndef _EASYSHADOW_LIBRARY_PAGE_H_
#define _EASYSHADOW_LIBRARY_PAGE_H_



namespace eshadow
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

#endif // _EASYSHADOW_LIBRARY_PAGE_H_