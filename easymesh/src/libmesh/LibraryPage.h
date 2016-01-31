#ifndef _EASYMESH_LIBRARY_PAGE_H_
#define _EASYMESH_LIBRARY_PAGE_H_



namespace emesh
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

#endif // _EASYMESH_LIBRARY_PAGE_H_