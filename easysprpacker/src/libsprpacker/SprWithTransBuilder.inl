#ifndef _EASYSPRPACKER_SPR_WITH_TRANS_BUILDER_INL_
#define _EASYSPRPACKER_SPR_WITH_TRANS_BUILDER_INL_

#include "typedef.h"

namespace esprpacker
{

template <typename TSymbol, typename TSprite, typename TPack>
SprWithTransBuilder<TSymbol, TSprite, TPack>::~SprWithTransBuilder()
{
	Clear();
}

template <typename TSymbol, typename TSprite, typename TPack>
void SprWithTransBuilder<TSymbol, TSprite, TPack>::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::map<const TSymbol*, std::vector<Item<TPack> > >::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		for (int i = 0, n = itr->second.size(); i < n; ++i) {
			bool has_next;
			visitor.Visit(const_cast<TPack*>(itr->second[i].node), has_next);
			if (!has_next) {
				return;
			}
		}
	}
}

template <typename TSymbol, typename TSprite, typename TPack>
void SprWithTransBuilder<TSymbol, TSprite, TPack>::Clear()
{
	std::map<const TSymbol*, std::vector<Item<TPack> > >::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		for (int i = 0, n = itr->second.size(); i < n; ++i) {
			itr->second[i].node->RemoveReference();
		}
	}
	m_map_data.clear();
}

template <typename TSymbol, typename TSprite, typename TPack>
const PackNode* SprWithTransBuilder<TSymbol, TSprite, TPack>::Create(const TSprite* spr)
{
	PackTrans trans(*spr);

	const TSymbol* sym = dynamic_cast<const TSymbol*>(spr->GetSymbol());
	std::map<const TSymbol*, std::vector<Item<TPack> > >::iterator itr
		= m_map_data.find(sym);
	if (itr != m_map_data.end()) 
	{
		for (int i = 0, n = itr->second.size(); i < n; ++i) {
			if (itr->second[i].trans == trans) {
				return itr->second[i].node;
			}
		}
	}

	sym->AddReference();
	TPack* node = new TPack(spr);
	node->SetFilepath(SPRITE_FILEPATH);
	node->SetID(sym->GetFilepath());
	node->AddReference();
	if (itr == m_map_data.end()) {
		std::vector<Item<TPack> > items;
		items.push_back(Item<TPack>(trans, node));
		m_map_data.insert(std::make_pair(sym, items));
	} else {
		itr->second.push_back(Item<TPack>(trans, node));
	}
	return node;
}

}

#endif // _EASYSPRPACKER_SPR_WITH_TRANS_BUILDER_INL_
