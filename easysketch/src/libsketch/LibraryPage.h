#ifndef _LIBSKETCH_LIBRARY_PAGE_H_
#define _LIBSKETCH_LIBRARY_PAGE_H_


#include <easy3d.h>

namespace libsketch
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(ee::Symbol* symbol) const;

	virtual void LoadDefaultSymbol();

protected:
	virtual void OnAddPress(wxCommandEvent& event);

private:
	void LoadSymbol(e3d::ISurface* surface, const char* name);

}; // LibraryPage

}

#endif // _LIBSKETCH_LIBRARY_PAGE_H_