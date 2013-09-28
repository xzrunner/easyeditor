#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryMeshPage : public ILibraryPage
	{
	public:
		LibraryMeshPage(wxWindow* parent);

		virtual bool isHandleSymbol(ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // LibraryMeshPage
}

