#ifndef RAIDEN_LIBRARY_SHAPE_PAGE_H
#define RAIDEN_LIBRARY_SHAPE_PAGE_H

#include <drag2d.h>

namespace raiden
{
	class LibraryShapePage : public d2d::ILibraryPage
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

#endif // RAIDEN_LIBRARY_SHAPE_PAGE_H