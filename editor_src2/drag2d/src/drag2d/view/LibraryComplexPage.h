#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryComplexPage : public ILibraryPage
	{
	public:
		LibraryComplexPage(wxWindow* parent);

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // LibraryComplexPage
}

