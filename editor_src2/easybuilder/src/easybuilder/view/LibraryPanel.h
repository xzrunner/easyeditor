
#ifndef EBUILDER_LIBRARY_PANEL_H
#define EBUILDER_LIBRARY_PANEL_H

#include <drag2d.h>

namespace ebuilder
{
	class Actor;
	class ActorsPage;
	class LayersPage;
	class ScenesPage;
	class MediaPage;
	class BehaviorsPage;

	class LibraryPanel : public d2d::LibraryPanel
	{
	public:
		LibraryPanel(wxWindow* parent);

//		void addActor(Actor* actor);

		void clearUIListeners();

		d2d::LibraryList* getSceneList();
		d2d::LibraryList* getLayerList();
		d2d::LibraryList* getActorList();
		d2d::LibraryList* getMediaList();
		d2d::LibraryList* getBehaviorList();

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
