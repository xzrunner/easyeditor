#ifndef _EASYBUILDER_LIBGDX_PAGE_H_
#define _EASYBUILDER_LIBGDX_PAGE_H_

#include "../CodePage.h"

namespace ebuilder
{
namespace libgdx
{

class Page : public CodePage
{
public:
	Page(wxWindow* parent, const std::string& name)
		: CodePage(parent, name) {
			InitializePrefs("Java");
	}

}; // Page

}
}

#endif // _EASYBUILDER_LIBGDX_PAGE_H_
