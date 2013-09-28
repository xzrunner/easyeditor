#pragma once

#include <drag2d.h>

namespace eanim
{
	class LibrarySymbolPage : public d2d::ILibraryPage
	{
	public:
		LibrarySymbolPage(wxWindow* parent);

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // LibrarySymbolPage
}

