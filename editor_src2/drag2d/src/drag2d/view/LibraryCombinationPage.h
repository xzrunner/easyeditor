#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryCombinationPage : public ILibraryPage
	{
	public:
		LibraryCombinationPage(wxWindow* parent);

		virtual bool isHandleSymbol(ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // LibraryCombinationPage
}

