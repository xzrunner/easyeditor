#ifndef _EASYAUDIO_LIBRARY_PAGE_H_
#define _EASYAUDIO_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace eaudio
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(ee::Symbol* sym) const;

}; // LibraryPage

}

#endif // _EASYAUDIO_LIBRARY_PAGE_H_