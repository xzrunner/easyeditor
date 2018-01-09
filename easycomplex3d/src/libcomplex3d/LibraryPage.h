#ifndef _EASYCOMPLEX3D_LIBRARY_PAGE_H_
#define _EASYCOMPLEX3D_LIBRARY_PAGE_H_

#include <easynode3d/LibraryPage.h>

namespace n3 { class Surface; }

namespace ecomplex3d
{

class LibraryPage : public enode3d::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual void LoadDefaultSymbol() override;

private:
	void LoadSurface(n3::Surface* surface, const char* name);

}; // LibraryPage

}

#endif // _EASYCOMPLEX3D_LIBRARY_PAGE_H_
