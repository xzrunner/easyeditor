#include "LayersMgr.h"
#include "Layer.h"

namespace eanim
{

LayersMgr::~LayersMgr()
{
	clear();
}

void LayersMgr::loadFromTextFile(std::ifstream& fin)
{
	clear();

	std::string flag;
	size_t size;
	fin >> flag >> size;
	for (size_t i = 0; i < size; ++i)
	{
		Layer* layer = new Layer;
		layer->loadFromTextFile(fin);
		m_layers.push_back(layer);
	}
}

void LayersMgr::storeToTextFile(std::ofstream& fout) const
{
	fout << "layer " << m_layers.size() << '\n';
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
		m_layers[i]->storeToTextFile(fout);
}

void LayersMgr::insertNewLayer()
{
	m_layers.push_back(new Layer);
}

void LayersMgr::deleteLayer(int index)
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

size_t LayersMgr::size() const
{
	return m_layers.size();
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

void LayersMgr::removeSprite(d2d::ISprite* sprite)
{
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		if (m_layers[i]->removeSprite(sprite))
			break;
	}
}

} // eanim