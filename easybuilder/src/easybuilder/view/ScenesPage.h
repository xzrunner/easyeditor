
#ifndef EBUILDER_SCENES_PAGE_H
#define EBUILDER_SCENES_PAGE_H

#include <drag2d.h>

namespace ebuilder
{
	class ListObserver;
	class Behavior;

	class ScenesPage : public d2d::ILibraryPage
	{
	public:
		ScenesPage(wxWindow* parent);
		virtual ~ScenesPage();

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

		ListObserver* getListObserver() {
			return m_observer;
		}

		void insert();

		void enableDelBtn();

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event);
		virtual void onDelPress(wxCommandEvent& event);

	private:
		ListObserver* m_observer;

	}; // ScenesPage
}

#endif // EBUILDER_SCENES_PAGE_H
