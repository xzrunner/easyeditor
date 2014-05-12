
#ifndef EBUILDER_BEHAVIORS_PAGE_H
#define EBUILDER_BEHAVIORS_PAGE_H

#include <drag2d.h>

namespace ebuilder
{
	class BehaviorsPage : public d2d::ILibraryPage
	{
	public:
		BehaviorsPage(wxWindow* parent);

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

		virtual void clear();

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event);

	private:
		void initBehaviors();

	}; // BehaviorsPage
}

#endif // EBUILDER_BEHAVIORS_PAGE_H
