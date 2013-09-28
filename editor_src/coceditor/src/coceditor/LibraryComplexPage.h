#pragma once

#include <drag2d.h>

namespace coceditor
{
	class LibraryComplexPage : public d2d::ILibraryPage
	{
	public:
		LibraryComplexPage(wxWindow* parent);

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event);

	private:
		void onNewBtnPress(wxCommandEvent& event);

	}; // LibraryComplexPage
}

