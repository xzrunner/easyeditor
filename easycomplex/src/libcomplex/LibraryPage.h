#ifndef _EASYCOMPLEX_LIBRARY_PAGE_H_
#define _EASYCOMPLEX_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace ecomplex
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(ee::Symbol* symbol) const;

	virtual bool LoadFromConfig();

protected:
	virtual void OnAddPress(wxCommandEvent& event);

private:
	void loadFromJsonFile(const std::string& filename);
	void loadFromLuaFile(const std::string& filename);

}; // LibraryPage

}

#endif // _EASYCOMPLEX_LIBRARY_PAGE_H_