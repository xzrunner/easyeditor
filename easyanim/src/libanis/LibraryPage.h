#ifndef _LIBANIS_LIBRARY_PAGE_H_
#define _LIBANIS_LIBRARY_PAGE_H_



namespace libanis
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

#endif // _LIBANIS_LIBRARY_PAGE_H_