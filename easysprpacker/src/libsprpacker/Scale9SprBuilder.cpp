#include "Scale9SprBuilder.h"
#include "PackScale9Spr.h"

#include <easyscale9.h>

namespace esprpacker
{

SINGLETON_DEFINITION(Scale9SprBuilder);

Scale9SprBuilder::Scale9SprBuilder()
{
}

Scale9SprBuilder::~Scale9SprBuilder()
{
	Clear();
}

void Scale9SprBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::multimap<const escale9::Symbol*, const PackScale9Spr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackScale9Spr*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void Scale9SprBuilder::Clear()
{
	std::multimap<const escale9::Symbol*, const PackScale9Spr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* Scale9SprBuilder::Create(const escale9::Sprite* spr)
{
	sm::vec2 sz = spr->GetScale9().GetSize();
	const escale9::Symbol* key = dynamic_cast<const escale9::Symbol*>(spr->GetSymbol());
	std::multimap<const escale9::Symbol*, const PackScale9Spr*>::const_iterator 
		itr_s = m_map_data.lower_bound(key),
		itr_e = m_map_data.upper_bound(key),
		itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		if (itr->second->GetSize() == sz) {
			itr->second->AddReference();
			return itr->second;
		}
	}

	key->AddReference();
	PackScale9Spr* node = new PackScale9Spr(spr);
	node->AddReference();
	m_map_data.insert(std::make_pair(key, node));
	return node;
}

}