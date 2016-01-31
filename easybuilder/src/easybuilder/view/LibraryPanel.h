
#ifndef EBUILDER_LIBRARY_PANEL_H
#define EBUILDER_LIBRARY_PANEL_H



namespace ebuilder
{
	class Actor;
	class ActorsPage;
	class LayersPage;
	class ScenesPage;
	class MediaPage;
	class BehaviorsPage;

	class LibraryPanel : public ee::LibraryPanel
	{
	public:
		LibraryPanel(wxWindow* parent);

//		void addActor(Actor* actor);

		void clearUIListeners();

		ee::LibraryList* getSceneList();
		ee::LibraryList* getLayerList();
		ee::LibraryList* getActorList();
		ee::LibraryList* getMediaList();
		ee::LibraryList* getBehaviorList();

		LayersPage* getLayerPage() { return m_layerPage; }
		ScenesPage* getScenePage() { return m_scenePage; }
		MediaPage* getMediaPage() { return m_mediaPage; }

	private:
		void initLayout();

	private:
		void initTitle(wxSizer* sizer);
		void initNotebook(wxSizer* sizer);
		void initTabPages(wxSizer* sizer);

	private:
		ScenesPage* m_scenePage;
		LayersPage* m_layerPage;
		ActorsPage* m_actorPage;
		MediaPage* m_mediaPage;
		BehaviorsPage* m_behaviorPage;

	}; // LibraryPanel
}

#endif // EBUILDER_LIBRARY_PANEL_H
