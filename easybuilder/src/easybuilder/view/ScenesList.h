
#ifndef EBUILDER_SCENES_LIST_H
#define EBUILDER_SCENES_LIST_H

#include <drag2d.h>

namespace ebuilder
{
	class ScenesPage;

	class ScenesList : public d2d::LibraryList
	{
	public:
		ScenesList(ScenesPage* page);

		virtual void onListSelected(wxCommandEvent& event);
		virtual void onListDoubleClicked(wxCommandEvent& event);

	private:
		void onSelected(int index);

	private:
		ScenesPage* m_page;

	}; // ScenesList
}

#endif // EBUILDER_SCENES_LIST_H
