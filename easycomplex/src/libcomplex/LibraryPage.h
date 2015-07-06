#ifndef COMPLEX_LIBRARY_PAGE_H
#define COMPLEX_LIBRARY_PAGE_H

#include <drag2d.h>

namespace ecomplex
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(d2d::ISymbol* symbol) const;

	virtual void LoadFromConfig();

protected:
	virtual void OnAddPress(wxCommandEvent& event);

private:
	void loadFromJsonFile(const std::string& filename);
	void loadFromLuaFile(const std::string& filename);

}; // LibraryPage

}

#endif // COMPLEX_LIBRARY_PAGE_H