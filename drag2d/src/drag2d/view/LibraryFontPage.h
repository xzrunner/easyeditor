#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryFontPage : public ILibraryPage
	{
	public:
		LibraryFontPage(wxWindow* parent);

		virtual bool isHandleSymbol(ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // LibraryFontPage
}

