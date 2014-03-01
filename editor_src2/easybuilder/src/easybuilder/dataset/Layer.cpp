
#include "Layer.h"
#include "Actor.h"

using namespace ebuilder;

Layer::Layer()
	: isVisible(true)
	, isEditable(true)
{
	static int count = 0;
	name = wxString("layer") + wxString::FromDouble(count++);
}

Layer::~Layer()
{
	for_each(m_actors.begin(), m_actors.end(), DeletePointerFunctor<Actor>());
}

void Layer::insert(Actor* actor) 
{
	m_actors.push_back(actor);
}

void Layer::insert(d2d::TextSprite* text) 
{
	m_texts.push_back(text);
}

void Layer::remove(d2d::ISprite* sprite)
{
	{
		std::vector<Actor*>::iterator itr = find(m_actors.begin(), m_actors.end(), sprite);
		if (itr != m_actors.end())
		{
			(*itr)->release();
			m_actors.erase(itr);
			return;
		}
	}
	{
		std::vector<d2d::TextSprite*>::iterator itr = find(m_texts.begin(), m_texts.end(), sprite);
		if (itr != m_texts.end())
		{
			(*itr)->release();
			m_texts.erase(itr);
			return;
		}
	}
}

void Layer::resetOrder(d2d::ISprite* sprite, bool up)
{
	for (size_t i = 0, n = m_actors.size(); i < n; ++i)
	{
		if (m_actors[i] == sprite)
		{
			if (up && i != n - 1)
			{
				Actor* tmp = m_actors[i];
				m_actors[i] = m_actors[i+1];
				m_actors[i+1] = tmp;
			}
			else if (!up && i != 0)
			{
				Actor* tmp = m_actors[i];
				m_actors[i] = m_actors[i-1];
				m_actors[i-1] = tmp;
			}
		}
	}
}

void Layer::clear()
{
	for (size_t i = 0, n = m_actors.size(); i < n; ++i)
		m_actors[i]->release();
	m_actors.clear();
	for (size_t i = 0, n = m_texts.size(); i < n; ++i)
		m_texts[i]->release();
	m_texts.clear();
}
