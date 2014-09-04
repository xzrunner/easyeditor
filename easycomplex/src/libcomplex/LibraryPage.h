#ifndef COMPLEX_LIBRARY_PAGE_H
#define COMPLEX_LIBRARY_PAGE_H

#include <drag2d.h>

namespace ecomplex
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	virtual void LoadFromConfig();

protected:
	virtual void onAddPress(wxCommandEvent& event);

private:
	void loadFromJsonFile(const wxString& filename);
	void loadFromLuaFile(const wxString& filename);

}; // LibraryPage

}

#endif // COMPLEX_LIBRARY_PAGE_H