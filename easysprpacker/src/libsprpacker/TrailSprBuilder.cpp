#include "TrailSprBuilder.h"
#include "PackTrailSpr.h"

#include <easytrail.h>

namespace esprpacker
{

SINGLETON_DEFINITION(TrailSprBuilder);

TrailSprBuilder::TrailSprBuilder()
{
}

TrailSprBuilder::~TrailSprBuilder()
{
	Clear();
}

void TrailSprBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::multimap<const etrail::Symbol*, const PackTrailSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackTrailSpr*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void TrailSprBuilder::Clear()
{
	std::multimap<const etrail::Symbol*, const PackTrailSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* TrailSprBuilder::Create(const etrail::Sprite* spr)
{
	const etrail::Symbol* key = dynamic_cast<const etrail::Symbol*>(spr->GetSymbol());
	std::multimap<const etrail::Symbol*, const PackTrailSpr*>::const_iterator 
		itr_s = m_map_data.lower_bound(key),
		itr_e = m_map_data.upper_bound(key),
		itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		return itr->second;
	}

	key->AddReference();
	PackTrailSpr* node = new PackTrailSpr(spr);
	node->AddReference();
	m_map_data.insert(std::make_pair(key, node));
	return node;
}

}