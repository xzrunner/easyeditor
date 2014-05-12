#ifndef _ANIMATION_LIBRARY_PAGE_H_
#define _ANIMATION_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace anim
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

protected:
	virtual void onAddPress(wxCommandEvent& event);

private:
	void loadFromJsonFile(const wxString& filename);
	void loadFromLuaFile(const wxString& filename);

}; // LibraryPage

}

#endif // _ANIMATION_LIBRARY_PAGE_H_