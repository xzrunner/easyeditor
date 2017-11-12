#ifndef _EASYSKETCH_LIBRARY_PAGE_H_
#define _EASYSKETCH_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

#include <easy3d.h>

namespace esketch
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
	void LoadSymbol(e3d::ISurface* surface, const char* name);

}; // LibraryPage

}

#endif // _EASYSKETCH_LIBRARY_PAGE_H_