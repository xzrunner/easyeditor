
#ifndef EBUILDER_ACTORS_PAGE_H
#define EBUILDER_ACTORS_PAGE_H

#include <drag2d.h>

namespace ebuilder
{
	class ActorsPage : public d2d::ILibraryPage
	{
	public:
		ActorsPage(wxWindow* parent);

		//
		// d2d::ILibraryPage interface
		//
		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event);
		virtual void onDelPress(wxCommandEvent& event);

	protected:
		void initButtons(wxSizer* sizer);

	}; // ActorsPage
}

#endif // EBUILDER_ACTORS_PAGE_H
