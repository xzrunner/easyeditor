
#ifndef LIBBUILDER_CC_PAGE_H
#define LIBBUILDER_CC_PAGE_H

#include "../CodePage.h"

namespace ebuilder
{
	namespace cc
	{
		class Page : public CodePage
		{
		public:
			Page(wxWindow* parent, const wxString& name)
				: CodePage(parent, name) {
					InitializePrefs ("C++");
			}

		}; // Page
	}
}

#endif // LIBBUILDER_CC_PAGE_H
