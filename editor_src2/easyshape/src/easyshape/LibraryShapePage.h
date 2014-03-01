
#ifndef ESHAPE_LIBRARY_SHAPE_PAGE_H
#define ESHAPE_LIBRARY_SHAPE_PAGE_H

#include "LibraryPage.h"

namespace eshape
{
	class LibraryShapePage : public LibraryPage
	{
	public:
		LibraryShapePage(wxWindow* parent);

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event);

	private:
		void onNewBtnPress(wxCommandEvent& event);

	}; // LibraryShapePage
}

#endif // ESHAPE_LIBRARY_SHAPE_PAGE_H