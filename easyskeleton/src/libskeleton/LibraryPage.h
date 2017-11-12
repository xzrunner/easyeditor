#ifndef _LIBSKELETON_LIBRARY_PAGE_H_
#define _LIBSKELETON_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace libskeleton
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(const ee::SymPtr& sym) const override;

protected:
	virtual void OnAddPress(wxCommandEvent& event) override;

private:
	void LoadFromEasyFile(const std::string& filename);
	void LoadFromJsonFile(const std::string& filename);

}; // LibraryPage

}

#endif // _LIBSKELETON_LIBRARY_PAGE_H_
