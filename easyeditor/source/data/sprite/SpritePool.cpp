#include "SpritePool.h"
#include "Symbol.h"
#include "Sprite.h"

namespace ee
{

CU_SINGLETON_DEFINITION(SpritePool);

SpritePool::SpritePool()
{
}

void SpritePool::Insert(const Sprite& spr)
{
	auto itr = m_map_symbol2sprites.find(dynamic_cast<Symbol*>(spr.GetSymbol().get()));
	if (itr == m_map_symbol2sprites.end())
	{
		SpriteList list;
		list.push_back(&spr);
		m_map_symbol2sprites.insert(std::make_pair(dynamic_cast<Symbol*>(spr.GetSymbol().get()), list));
	}
	else 
	{
		itr->second.push_back(&spr);
	}
}

void SpritePool::Remove(const Sprite& spr)
{
	auto sym = dynamic_cast<Symbol*>(spr.GetSymbol().get());
	auto itr = m_map_symbol2sprites.find(sym);
	if (itr != m_map_symbol2sprites.end()) 
	{
		SpriteList::iterator itr_sprite = itr->second.begin();
		for ( ; itr_sprite != itr->second.end(); )
		{
			if (*itr_sprite == &spr)
				itr_sprite = itr->second.erase(itr_sprite);
			else
				++itr_sprite;
		}
	}
}

void SpritePool::UpdateBoundings(const Symbol& sym)
{
	auto itr = m_map_symbol2sprites.find(&sym);
	if (itr != m_map_symbol2sprites.end())
	{
		for (size_t i = 0, n = itr->second.size(); i < n; ++i)
			itr->second[i]->UpdateBounding();
	}
}

}