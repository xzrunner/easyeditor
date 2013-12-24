
#ifndef LIBBUILDER_LOVE2D_PAGE_H
#define LIBBUILDER_LOVE2D_PAGE_H

#include "../CodePage.h"

namespace ebuilder
{
namespace love2d
{
	class Page : public CodePage
	{
	public:
		Page(wxWindow* parent, const wxString& name)
			: CodePage(parent, name) {
				InitializePrefs ("Lua");
		}

	}; // Page
}
}

#endif // LIBBUILDER_LOVE2D_PAGE_H
