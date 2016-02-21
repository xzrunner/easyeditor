#ifndef _EASYBUILDER_E2D_PAGE_H_
#define _EASYBUILDER_E2D_PAGE_H_

#include "../CodePage.h"

namespace ebuilder
{
namespace e2d
{

class Page : public CodePage
{
public:
	Page(wxWindow* parent, const std::string& name)
		: CodePage(parent, name) {
		InitializePrefs ("C++"/*DEFAULT_LANGUAGE*/);
	}

}; // Page

}
}

#endif // _EASYBUILDER_E2D_PAGE_H_
