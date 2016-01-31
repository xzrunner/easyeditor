#pragma once



namespace eanim
{
	class LibraryPanel;
	class Symbol;

	class SkeletonSymbolList : public ee::VerticalImageList
	{
	public:
		SkeletonSymbolList(wxWindow* parent, LibraryPanel* library);

		void reloadAllTexture();

		Symbol* getSymbol(int index) const;

	private:

	}; // SkeletonSymbolList
}


