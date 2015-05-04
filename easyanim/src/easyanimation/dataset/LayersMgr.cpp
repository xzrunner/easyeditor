#include "LayersMgr.h"
#include "Layer.h"

#include "frame/Controller.h"

namespace eanim
{

LayersMgr::LayersMgr(Controller* ctrl)
	: m_ctrl(ctrl)
{
}

LayersMgr::~LayersMgr()
{
	clear();
}

void LayersMgr::newLayer()
{
	m_layers.push_back(new Layer(m_ctrl));
}

void LayersMgr::insertLayer(Layer* layer)
{
	m_ctrl->setCurrFrame(m_layers.size(), 1);
	m_layers.push_back(layer);
}

void LayersMgr::removeLayer(int index)
{
	if (index < 0 || index >= m_layers.size()) return;
	delete m_layers[index];
	m_layers.erase(m_layers.begin() + index);
}

void LayersMgr::changeLayerOrder(int from, int to)
{
	if (from < 0 || from >= m_layers.size()
		|| to < 0 || to >= m_layers.size()) 
		return;

	Layer* layer = m_layers[from];
	m_layers.erase(m_layers.begin() + from);
	m_layers.insert(m_layers.begin() + to, layer);
}

Layer* LayersMgr::getLayer(size_t index) const
{
	if (index < 0 || index >= m_layers.size()) return NULL;
	return m_layers[index];
}

int LayersMgr::getFrameCount() const
{
	int count = 0;
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		int tmp = layer->getFrameCount();
		if (tmp > count) count = tmp;
	}
	return count;
}

void LayersMgr::clear()
{
	for_each(m_layers.begin(), m_layers.end(), DeletePointerFunctor<Layer>());
	m_layers.clear();
}

// void LayersMgr::removeSprite(d2d::ISprite* sprite)
// {
// 	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
// 	{
// 		if (m_layers[i]->removeSprite(sprite))
// 			break;
// 	}
// }

} // eanim