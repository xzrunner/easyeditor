
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

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		virtual void onListSelected(wxCommandEvent& event);
		virtual void onListDoubleClicked(wxCommandEvent& event);

	private:
		void onSelected(int index);

	private:
		ScenesPage* m_page;

	}; // ScenesList
}

#endif // EBUILDER_SCENES_LIST_H
