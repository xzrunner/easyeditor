#ifndef _LIBANIM2_LIBRARY_PAGE_H_
#define _LIBANIM2_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace libanim2
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(ee::Symbol* sym) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

private:
	void LoadFromEasyFile(const std::string& filename);
	void LoadFromJsonFile(const std::string& filename);

}; // LibraryPage

}

#endif // _LIBANIM2_LIBRARY_PAGE_H_