
#ifndef EBUILDER_SCENES_LIST_H
#define EBUILDER_SCENES_LIST_H



namespace ebuilder
{
	class ScenesPage;

	class ScenesList : public ee::LibraryList
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
