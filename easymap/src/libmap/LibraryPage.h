#ifndef _EASYMAP_LIBRARY_PAGE_H_
#define _EASYMAP_LIBRARY_PAGE_H_



namespace emap
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

#endif // _EASYMAP_LIBRARY_PAGE_H_
