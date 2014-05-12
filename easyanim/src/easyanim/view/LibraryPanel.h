#pragma once

#include <drag2d.h>

namespace eanim
{
	class Symbol;
	class WholeSkeleton;
	class StagePanel;
	class LibrarySymbolPage;
	class LibrarySymbolList;
	class LibrarySkeletonPage;

	class LibraryPanel : public wxPanel, public d2d::ISerializable
	{
	public:
		static const wxString SYMBOL_LIST_NAME;
		static const wxString SKELETON_LIST_NAME;

	public:
		LibraryPanel(wxWindow* parent);

		//
		// d2d::ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		Symbol* getSymbol(int index) const;
		WholeSkeleton* getSkeleton(int index) const;

// 		int getIndex(Symbol* symbol) const;
// 		int getIndex(WholeSkeleton* skeleton) const;

		void reloadAllTexture();

		void clear();

	private:
		void initLayout();

		void initTitle(wxSizer* sizer);
		void initTabPages(wxSizer* sizer);

	private:
		LibrarySymbolPage* m_symbolPage;
		LibrarySkeletonPage* m_skeletonPage;

	}; // LibraryPanel
}

