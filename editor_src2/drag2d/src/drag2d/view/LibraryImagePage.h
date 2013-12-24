#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryImagePage : public ILibraryPage
	{
	public:
		LibraryImagePage(wxWindow* parent, bool supportBmp = true);

		virtual bool isHandleSymbol(ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	private:
		bool m_supportBmp;

	}; // LibraryImagePage
}

