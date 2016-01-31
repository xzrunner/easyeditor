
#include "Scene.h"
#include "Button.h"
#include "Behavior.h"

using namespace ebuilder::preview;

Scene::~Scene()
{
	for_each(m_items.begin(), m_items.end(), DeletePointerFunctor<Item>());
	m_items.clear();

	for_each(m_behaviors.begin(), m_behaviors.end(), DeletePointerFunctor<Behavior>());
	m_behaviors.clear();
}

Button* Scene::queryButtonByPos(const ee::Vector& pos) const
{
	for (size_t i = 0, n = m_items.size(); i < n; ++i)
	{
		Item* item = m_items[i];
		if (item->type() == Item::e_button && m_items[i]->isContain(pos))
			return static_cast<Button*>(m_items[i]);
	}
	return NULL;
}

void Scene::triggerAllBehaviors()
{
	for (size_t i = 0, n = m_behaviors.size(); i < n; ++i)
		m_behaviors[i]->trigger();
}
