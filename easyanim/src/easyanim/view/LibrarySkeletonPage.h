#pragma once

#include <drag2d.h>

namespace eanim
{
	class LibrarySkeletonPage : public d2d::ILibraryPage
	{
	public:
		LibrarySkeletonPage(wxWindow* parent);

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

 	protected:
 		virtual void onAddPress(wxCommandEvent& event);

	}; // LibrarySkeletonPage
}

