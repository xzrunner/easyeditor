
#ifndef EMODELING_LIBRARY_LIST_H
#define EMODELING_LIBRARY_LIST_H

#include <drag2d.h>

namespace emodeling
{
	class LibraryList : public d2d::LibraryList
	{
	public:
		LibraryList(d2d::ILibraryPage* page);

		virtual void onListDoubleClicked(wxCommandEvent& event);

	}; // LibraryPage
}

#endif // EMODELING_LIBRARY_LIST_H