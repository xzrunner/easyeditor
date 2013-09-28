#pragma once

#include "widgets/VerticalImageList.h"
#include "interfaces.h"

namespace d2d
{
	class ISymbol;

	class LibraryList : public VerticalImageList, public ISerializable
	{
	public:
		LibraryList(wxWindow* parent, const wxString& filter = wxT("symbol"),
			bool draggable = true);

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		ListItem* getItem(int index = -1) const;
		ISymbol* getSymbol(int index = -1) const;

		void reloadTexture() const;

	}; // LibraryList
}

