#ifndef _EASYBUILDER_LOVE2D_PAGE_H_
#define _EASYBUILDER_LOVE2D_PAGE_H_

#include "../CodePage.h"

namespace ebuilder
{
namespace love2d
{

class Page : public CodePage
{
public:
	Page(wxWindow* parent, const std::string& name)
		: CodePage(parent, name) {
			InitializePrefs ("Lua");
	}

}; // Page

}
}

#endif // _EASYBUILDER_LOVE2D_PAGE_H_
