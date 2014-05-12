
#ifndef EBUILDER_SCENE_H
#define EBUILDER_SCENE_H

#include <drag2d.h>

namespace ebuilder
{
	class Layer;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		void insert(Layer* layer);
		void remove(Layer* layer); 
		void swapLayers(int i0, int i1);

		const std::vector<Layer*>& getAllLayers() const {
			return m_layers;
		}

	public:
		wxString m_name;

		wxString m_tag;

		wxColor m_color;

	private:
		std::vector<Layer*> m_layers;

	}; // Scene
}

#endif // EBUILDER_SCENE_H
