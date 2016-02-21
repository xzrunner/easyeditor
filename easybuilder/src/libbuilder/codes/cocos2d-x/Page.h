#ifndef _EASYBUILDER_CC_PAGE_H_
#define _EASYBUILDER_CC_PAGE_H_

#include "../CodePage.h"

namespace ebuilder
{
namespace cc
{

class Page : public CodePage
{
public:
	Page(wxWindow* parent, const std::string& name)
		: CodePage(parent, name) {
			InitializePrefs ("C++");
	}

}; // Page

}
}

#endif // _EASYBUILDER_CC_PAGE_H_
