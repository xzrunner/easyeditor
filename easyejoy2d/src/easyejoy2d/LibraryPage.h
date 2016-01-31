#ifndef _EASYEJOY2D_LIBRARY_PAGE_H_
#define _EASYEJOY2D_LIBRARY_PAGE_H_



namespace eejoy2d
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

#endif // _EASYEJOY2D_LIBRARY_PAGE_H_