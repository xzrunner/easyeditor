#include "IconSprBuilder.h"
#include "PackIconSpr.h"

#include <easyicon.h>

namespace esprpacker
{

SINGLETON_DEFINITION(IconSprBuilder);

IconSprBuilder::IconSprBuilder()
{
}

IconSprBuilder::~IconSprBuilder()
{
	Clear();
}

void IconSprBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::multimap<const eicon::Symbol*, const PackIconSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackIconSpr*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void IconSprBuilder::Clear()
{
	std::multimap<const eicon::Symbol*, const PackIconSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* IconSprBuilder::Create(const eicon::Sprite* spr)
{
	float process = spr->GetProcess();
	const eicon::Symbol* key = dynamic_cast<const eicon::Symbol*>(spr->GetSymbol());
	std::multimap<const eicon::Symbol*, const PackIconSpr*>::const_iterator 
		itr_s = m_map_data.lower_bound(key),
		itr_e = m_map_data.upper_bound(key),
		itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		if (itr->second->GetProcess() == process) {
			itr->second->AddReference();
			return itr->second;
		}
	}

	key->AddReference();
	PackIconSpr* node = new PackIconSpr(spr);
	node->AddReference();
	m_map_data.insert(std::make_pair(key, node));
	return node;
}

}