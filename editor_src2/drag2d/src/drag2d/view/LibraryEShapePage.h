#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryEShapePage : public ILibraryPage
	{
	public:
		LibraryEShapePage(wxWindow* parent);

		virtual bool isHandleSymbol(ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // LibraryEShapePage
}

