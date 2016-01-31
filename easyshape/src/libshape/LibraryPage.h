#ifndef _LIBSHAPE_LIBRARY_PAGE_H_
#define _LIBSHAPE_LIBRARY_PAGE_H_



namespace libshape
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

#endif // _LIBSHAPE_LIBRARY_PAGE_H_