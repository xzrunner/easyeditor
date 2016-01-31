#ifndef _EASYEDITOR_LIBRARY_IMAGE_PAGE_H_
#define _EASYEDITOR_LIBRARY_IMAGE_PAGE_H_

#include "LibraryPage.h"

namespace ee
{

class LibraryImagePage : public LibraryPage
{
public:
	LibraryImagePage(wxWindow* parent, bool supportBmp = true);

	virtual bool IsHandleSymbol(Symbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

private:
	bool m_support_bmp;

}; // LibraryImagePage

}

#endif // _EASYEDITOR_LIBRARY_IMAGE_PAGE_H_