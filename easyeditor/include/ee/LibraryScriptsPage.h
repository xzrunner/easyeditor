#ifndef _EASYEDITOR_LIBRARY_SCRIPTS_PAGE_H_
#define _EASYEDITOR_LIBRARY_SCRIPTS_PAGE_H_

#include "LibraryPage.h"

namespace ee
{

class LibraryScriptsPage : public LibraryPage
{
public:
	LibraryScriptsPage(wxWindow* parent);

	virtual bool IsHandleSymbol(const SymPtr& sym) const override;

	virtual bool LoadFromConfig() override;

protected:
	virtual void OnAddPress(wxCommandEvent& event) override;

}; // LibraryScriptsPage

}

#endif // _EASYEDITOR_LIBRARY_SCRIPTS_PAGE_H_