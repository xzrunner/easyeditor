#ifndef _LIBANIMATION_LIBRARY_PAGE_H_
#define _LIBANIMATION_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace libanim
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(d2d::ISymbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

private:
	void loadFromJsonFile(const wxString& filename);
	void loadFromLuaFile(const wxString& filename);

}; // LibraryPage

}

#endif // _LIBANIMATION_LIBRARY_PAGE_H_