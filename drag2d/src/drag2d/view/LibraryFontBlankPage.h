#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryFontBlankPage : public ILibraryPage
	{
	public:
		LibraryFontBlankPage(wxWindow* parent);

		virtual bool IsHandleSymbol(ISymbol* symbol) const;

		virtual bool LoadFromConfig();

	protected:
		virtual void InitLayout(bool draggable = true);

	protected:
		virtual void OnAddPress(wxCommandEvent& event);

	private:
		void onNewBtnPress(wxCommandEvent& event);

	}; // LibraryFontBlankPage
}

