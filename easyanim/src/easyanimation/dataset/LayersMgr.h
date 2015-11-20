#ifndef _EASYANIM_LAYERS_MGR_H_
#define _EASYANIM_LAYERS_MGR_H_

#include <drag2d.h>

namespace eanim
{

class Controller;
class Layer;

class LayersMgr : public d2d::Observer
{
public:
	LayersMgr(Controller* ctrl);
	~LayersMgr();

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	void changeLayerOrder(int from, int to);

	size_t size() const {
		return m_layers.size();
	}

	Layer* getLayer(size_t index) const;

	int getFrameCount() const;

	bool clear();

//		void removeSprite(d2d::ISprite* sprite);

	const std::vector<Layer*>& getAllLayers() const {
		return m_layers;
	}

	int GetMaxFrame() const;

private:
	void Insert(Layer* layer);
	void Remove();

private:
	Controller* m_ctrl;

	std::vector<Layer*> m_layers;

}; // LayersMgr

}

#endif // _EASYANIM_LAYERS_MGR_H_