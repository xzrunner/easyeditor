#ifndef _EASYSPRPACKER_SPR_BUILDER_INL_
#define _EASYSPRPACKER_SPR_BUILDER_INL_

#include "typedef.h"

namespace esprpacker
{

template <typename TSymbol, typename TSprite, typename TPack>
SprBuilder<TSymbol, TSprite, TPack>::~SprBuilder()
{
	Clear();
}

template <typename TSymbol, typename TSprite, typename TPack>
void SprBuilder<TSymbol, TSprite, TPack>::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::multimap<const TSymbol*, const TPack*>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<TPack*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

template <typename TSymbol, typename TSprite, typename TPack>
void SprBuilder<TSymbol, TSprite, TPack>::Clear()
{
	std::multimap<const TSymbol*, const TPack*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

template <typename TSymbol, typename TSprite, typename TPack>
const PackNode* SprBuilder<TSymbol, TSprite, TPack>::Create(const TSprite* spr)
{
	const TSymbol* key = dynamic_cast<const TSymbol*>(spr->GetSymbol());
	std::multimap<const TSymbol*, const TPack*>::const_iterator 
		itr_s = m_map_data.lower_bound(key),
		itr_e = m_map_data.upper_bound(key),
		itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		if (itr->second->Equal(spr)) {
			itr->second->AddReference();
			return itr->second;
		}
	}

	key->AddReference();
	TPack* node = new TPack(spr);
	node->SetFilepath(SPRITE_FILEPATH);
	node->SetID(dynamic_cast<const ee::Symbol*>(spr->GetSymbol())->GetFilepath());
	node->AddReference();
	m_map_data.insert(std::make_pair(key, node));
	return node;
}

}

#endif // _EASYSPRPACKER_SPR_BUILDER_INL_
