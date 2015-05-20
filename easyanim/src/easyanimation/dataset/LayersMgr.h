#pragma once

#include <drag2d.h>

namespace eanim
{
	class Controller;
	class Layer;

	class LayersMgr
	{
	public:
		LayersMgr(Controller* ctrl);
		~LayersMgr();
 
		Layer* newLayer();
		void insertLayer(Layer* layer);
		void removeLayer(int index);
		void changeLayerOrder(int from, int to);

		size_t size() const {
			return m_layers.size();
		}

		Layer* getLayer(size_t index) const;

		int getFrameCount() const;

		void clear();

//		void removeSprite(d2d::ISprite* sprite);

		const std::vector<Layer*>& getAllLayers() const {
			return m_layers;
		}

		int GetMaxFrame() const;

	private:
		Controller* m_ctrl;

		std::vector<Layer*> m_layers;

	}; // LayersMgr
}

