#ifndef RAIDEN_LIBRARY_SHAPE_LIST_H
#define RAIDEN_LIBRARY_SHAPE_LIST_H

#include <drag2d.h>

namespace raiden
{
	class LibraryShapeList : public d2d::LibraryList
	{
	public:
		LibraryShapeList(wxWindow* parent);

		virtual void onListDoubleClicked(wxCommandEvent& event);

	}; // LibraryShapeList
}

#endif // RAIDEN_LIBRARY_SHAPE_LIST_H