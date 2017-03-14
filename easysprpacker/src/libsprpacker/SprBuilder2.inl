#ifndef _EASYSPRPACKER_SPR_BUILDER2_INL_
#define _EASYSPRPACKER_SPR_BUILDER2_INL_

#include "typedef.h"

namespace esprpacker
{

template <typename TSprite, typename TPack>
SprBuilder2<TSprite, TPack>::~SprBuilder2()
{
	Clear();
}

template <typename TSprite, typename TPack>
void SprBuilder2<TSprite, TPack>::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::map<const TSprite*, const TPack*>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<TPack*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

template <typename TSprite, typename TPack>
void SprBuilder2<TSprite, TPack>::Clear()
{
	std::map<const TSprite*, const TPack*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

template <typename TSprite, typename TPack>
const PackNode* SprBuilder2<TSprite, TPack>::Create(const TSprite* spr)
{
	std::map<const TSprite*, const TPack*>::iterator
		itr = m_map_data.find(spr);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	TPack* node = new TPack(spr);
	node->SetFilepath(SPRITE_FILEPATH);
	node->SetID(dynamic_cast<const ee::Symbol*>(spr->GetSymbol())->GetFilepath());
	spr->AddReference();
	m_map_data.insert(std::make_pair(spr, node));
	node->AddReference();
	return node;
}

}

#endif // _EASYSPRPACKER_SPR_BUILDER2_INL_
