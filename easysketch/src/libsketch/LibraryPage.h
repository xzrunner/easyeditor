#ifndef _LIBSKETCH_LIBRARY_PAGE_H_
#define _LIBSKETCH_LIBRARY_PAGE_H_

#include <drag2d.h>
#include <easy3d.h>

namespace libsketch
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(d2d::ISymbol* symbol) const;

	virtual void LoadDefaultSymbol();

protected:
	virtual void OnAddPress(wxCommandEvent& event);

private:
	void LoadSymbol(e3d::ISurface* surface, const char* name);

}; // LibraryPage

}

#endif // _LIBSKETCH_LIBRARY_PAGE_H_