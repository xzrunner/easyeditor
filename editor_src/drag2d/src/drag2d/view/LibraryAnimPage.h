#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryAnimPage : public ILibraryPage
	{
	public:
		LibraryAnimPage(wxWindow* parent);

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // LibraryAnimPage
}

