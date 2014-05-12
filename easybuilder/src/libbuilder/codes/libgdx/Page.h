
#ifndef LIBBUILDER_LIBGDX_PAGE_H
#define LIBBUILDER_LIBGDX_PAGE_H

#include "../CodePage.h"

namespace ebuilder
{
	namespace libgdx
	{
		class Page : public CodePage
		{
		public:
			Page(wxWindow* parent, const wxString& name)
				: CodePage(parent, name) {
					InitializePrefs("Java");
			}

		}; // Page
	}
}

#endif // LIBBUILDER_LIBGDX_PAGE_H
