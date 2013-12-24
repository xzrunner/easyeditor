#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryShapePage : public ILibraryPage
	{
	public:
		LibraryShapePage(wxWindow* parent);

		virtual bool isHandleSymbol(ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // LibraryShapePage
}

