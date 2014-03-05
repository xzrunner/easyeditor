#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryScale9Page : public ILibraryPage
	{
	public:
		LibraryScale9Page(wxWindow* parent);

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // LibraryScale9Page
}

