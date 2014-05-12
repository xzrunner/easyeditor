
#include "Scene.h"
#include "Layer.h"

using namespace ebuilder;

Scene::Scene()
{
	static int count = 0;
	m_name = wxString("scene") + wxString::FromDouble(count++);

	m_color = *wxWHITE;
}

Scene::~Scene()
{
	for_each(m_layers.begin(), m_layers.end(), DeletePointerFunctor<Layer>());
}

void Scene::insert(Layer* layer)
{
	m_layers.push_back(layer);
}

void Scene::remove(Layer* layer)
{
	for (size_t i = 0, n = m_layers.size(); i < n; ++i)
	{
		if (m_layers[i] == layer)
		{
			m_layers.erase(m_layers.begin() + i);
			break;
		}
	}
}

void Scene::swapLayers(int i0, int i1)
{
	if (i0 < 0 || i0 >= m_layers.size() ||
		i1 < 0 || i1 >= m_layers.size())
		return;

	Layer* tmp = m_layers[i0];
	m_layers[i0] = m_layers[i1];
	m_layers[i1] = tmp;
}