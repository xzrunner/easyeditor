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
	auto itr = m_map_data.begin();
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
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

template <typename TSymbol, typename TSprite, typename TPack>
const PackNode* SprBuilder<TSymbol, TSprite, TPack>::Create(const std::shared_ptr<TSprite>& spr)
{
	const TSymbol* key = dynamic_cast<const TSymbol*>(spr->GetSymbol());
	auto itr_s = m_map_data.lower_bound(key);
	auto itr_e = m_map_data.upper_bound(key);
	std::multimap<const std::shared_ptr<TSymbol>, const TPack*>::iterator itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		if (itr->second->Equal(spr)) {
			itr->second->AddReference();
			return itr->second;
		}
	}

	key->AddReference();
	TPack* node = new TPack(spr);
	node->SetFilepath(SPRITE_FILEPATH);
	node->SetID(std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->GetFilepath());
	node->AddReference();
	m_map_data.insert(std::make_pair(key, node));
	return node;
}

}

#endif // _EASYSPRPACKER_SPR_BUILDER_INL_
