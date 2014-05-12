
#ifndef LIBBUILDER_E2D_PAGE_H
#define LIBBUILDER_E2D_PAGE_H

#include "../CodePage.h"

namespace ebuilder
{
	namespace e2d
	{
		class Page : public CodePage
		{
		public:
			Page(wxWindow* parent, const wxString& name)
				: CodePage(parent, name) {
				InitializePrefs ("C++"/*DEFAULT_LANGUAGE*/);
			}

		}; // Page
	}
}

#endif // LIBBUILDER_E2D_PAGE_H
