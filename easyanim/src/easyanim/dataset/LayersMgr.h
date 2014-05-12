#pragma once

#include <drag2d.h>

namespace eanim
{
	class Layer;

	class LayersMgr
	{
	public:
		~LayersMgr();
 
		void loadFromTextFile(std::ifstream& fin);
		void storeToTextFile(std::ofstream& fout) const;

		void insertNewLayer();
		void deleteLayer(int index);
		void changeLayerOrder(int from, int to);

		size_t size() const;

		Layer* getLayer(size_t index) const;

		int getFrameCount() const;

		void clear();

		void removeSprite(d2d::ISprite* sprite);

	private:
		std::vector<Layer*> m_layers;

	}; // LayersMgr
}

