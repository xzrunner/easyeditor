#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryFontBlankPage : public ILibraryPage
	{
	public:
		LibraryFontBlankPage(wxWindow* parent);

		virtual bool isHandleSymbol(ISymbol* symbol) const;

	protected:
		virtual void initLayout(bool draggable = true);

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	private:
		void onNewBtnPress(wxCommandEvent& event);

		void LoadFromConfig();

	}; // LibraryFontBlankPage
}

