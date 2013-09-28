#pragma once

#include "LibraryList.h"

namespace d2d
{
	class LibraryFontBlankList : public LibraryList
	{
	public:
		LibraryFontBlankList(wxWindow* parent);

		virtual void onListDoubleClicked(wxCommandEvent& event);

	}; // LibraryFontBlankList
}

