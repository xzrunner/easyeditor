#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class Library9PatchPage : public ILibraryPage
	{
	public:
		Library9PatchPage(wxWindow* parent);

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // Library9PatchPage
}

