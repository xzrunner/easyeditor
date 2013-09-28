#pragma once

#include <drag2d.h>

namespace coceditor
{
	class LibraryComplexList : public d2d::LibraryList
	{
	public:
		LibraryComplexList(wxWindow* parent);

		virtual void onListDoubleClicked(wxCommandEvent& event);

	}; // LibraryComplexList 
}

