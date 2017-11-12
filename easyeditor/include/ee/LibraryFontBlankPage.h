#ifndef _EASYEDITOR_LIBRARY_FONTBLANK_PAGE_H_
#define _EASYEDITOR_LIBRARY_FONTBLANK_PAGE_H_

#include "LibraryPage.h"

namespace ee
{

class LibraryFontBlankPage : public LibraryPage
{
public:
	LibraryFontBlankPage(wxWindow* parent);

	virtual bool IsHandleSymbol(const SymPtr& sym) const override;

	virtual bool LoadFromConfig() override;

protected:
	virtual void InitLayout(bool draggable = true) override;

protected:
	virtual void OnAddPress(wxCommandEvent& event) override;

private:
	void OnNewBtnPress(wxCommandEvent& event);

}; // LibraryFontBlankPage

}

#endif // _EASYEDITOR_LIBRARY_FONTBLANK_PAGE_H_