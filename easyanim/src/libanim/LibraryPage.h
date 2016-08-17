#ifndef _EASYANIM_LIBRARY_PAGE_H_
#define _EASYANIM_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace eanim
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(ee::Symbol* sym) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

private:
	void LoadFromJsonFile(const std::string& filename);
	void LoadFromLuaFile(const std::string& filename);

}; // LibraryPage

}

#endif // _EASYANIM_LIBRARY_PAGE_H_