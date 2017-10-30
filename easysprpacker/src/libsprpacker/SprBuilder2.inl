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
	auto itr = m_map_data.begin();
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
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

template <typename TSprite, typename TPack>
const PackNode* SprBuilder2<TSprite, TPack>::Create(const std::shared_ptr<const TSprite>& spr, bool force_curr)
{
	auto itr = m_map_data.find(spr);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	TPack* node = new TPack(std::const_pointer_cast<TSprite>(spr));
	node->SetFilepath(SPRITE_FILEPATH);
	node->SetID(std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->GetFilepath(), force_curr);
	m_map_data.insert(std::make_pair(spr, node));
	node->AddReference();
	return node;
}

}

#endif // _EASYSPRPACKER_SPR_BUILDER2_INL_
