#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryFontPage : public ILibraryPage
	{
	public:
		LibraryFontPage(wxWindow* parent);

		virtual bool IsHandleSymbol(ISymbol* symbol) const;

	protected:
		virtual void OnAddPress(wxCommandEvent& event);

	}; // LibraryFontPage
}

