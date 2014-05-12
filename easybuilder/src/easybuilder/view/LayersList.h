
#ifndef EBUILDER_LAYERS_LIST_H
#define EBUILDER_LAYERS_LIST_H

#include <drag2d.h>

namespace ebuilder
{
	class Scene;
	class LayersPage;

	class LayersList : public d2d::LibraryList
	{
	public:
		LayersList(LayersPage* page);

		virtual void onListSelected(wxCommandEvent& event);
		virtual void onListDoubleClicked(wxCommandEvent& event);

		void loadFromScene(Scene* scene);

	private:
		void onSelected(int index);

	private:
		LayersPage* m_page;

	}; // LayersList
}

#endif // EBUILDER_LAYERS_LIST_H