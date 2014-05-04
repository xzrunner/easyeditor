#pragma once

#include "widgets/VerticalImageList.h"
#include "interfaces.h"

namespace d2d
{
	class ISymbol;

	class LibraryList : public VerticalImageList
	{
	public:
		LibraryList(wxWindow* parent, const wxString& filter = wxT("symbol"),
			bool draggable = true);

		ListItem* getItem(int index = -1) const;
		ISymbol* getSymbol(int index = -1) const;

		void reloadTexture() const;

	}; // LibraryList
}

