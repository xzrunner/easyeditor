#pragma once

#include <drag2d.h>

namespace eanim
{
	class LibraryPanel;
	class Symbol;

	class SkeletonSymbolList : public d2d::VerticalImageList
	{
	public:
		SkeletonSymbolList(wxWindow* parent, LibraryPanel* library);

		void reloadAllTexture();

		Symbol* getSymbol(int index) const;

	private:

	}; // SkeletonSymbolList
}


