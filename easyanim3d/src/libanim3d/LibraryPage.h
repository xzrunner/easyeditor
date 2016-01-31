#ifndef _EASYANIM3D_LIBRARY_PAGE_H_
#define _EASYANIM3D_LIBRARY_PAGE_H_



namespace eanim3d
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

#endif // _EASYANIM3D_LIBRARY_PAGE_H_