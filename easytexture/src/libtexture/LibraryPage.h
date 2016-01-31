#ifndef _EASYTEXTURE_LIBRARY_PAGE_H_
#define _EASYTEXTURE_LIBRARY_PAGE_H_



namespace etexture
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

#endif // _EASYTEXTURE_LIBRARY_PAGE_H_
