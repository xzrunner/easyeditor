#pragma once

#include "ILibraryPage.h"

namespace d2d
{
	class LibraryImagePage : public ILibraryPage
	{
	public:
		LibraryImagePage(wxWindow* parent, bool supportBmp = true);

		virtual bool IsHandleSymbol(Symbol* symbol) const;

	protected:
		virtual void OnAddPress(wxCommandEvent& event);

	private:
		bool m_supportBmp;

	}; // LibraryImagePage
}

