#ifndef _EASYCOMPLEX3D_LIBRARY_PAGE_H_
#define _EASYCOMPLEX3D_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace n3 { class Surface; }

namespace ecomplex3d
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(const ee::SymPtr& sym) const override;

	virtual void LoadDefaultSymbol() override;

protected:
	virtual void OnAddPress(wxCommandEvent& event) override;

private:
	void LoadSymbol(n3::Surface* surface, const char* name);

}; // LibraryPage

}

#endif // _EASYCOMPLEX3D_LIBRARY_PAGE_H_
