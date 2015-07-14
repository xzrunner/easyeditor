#pragma once

#include "LibraryList.h"

namespace d2d
{
	class LibraryFontBlankList : public LibraryList
	{
	public:
		LibraryFontBlankList(ILibraryPage* page);

		virtual void OnListDoubleClicked(wxCommandEvent& event);

	}; // LibraryFontBlankList
}

