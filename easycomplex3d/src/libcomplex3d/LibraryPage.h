#ifndef _EASYCOMPLEX3D_LIBRARY_PAGE_H_
#define _EASYCOMPLEX3D_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace ecomplex3d
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(const ee::SymPtr& sym) const override;

	virtual void LoadDefaultSymbol() override;

private:
	void LoadGeometric(const std::string& name);

}; // LibraryPage

}

#endif // _EASYCOMPLEX3D_LIBRARY_PAGE_H_
