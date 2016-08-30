#include "Scale9Builder.h"
#include "PackScale9.h"

#include <easyscale9.h>

namespace esprpacker
{

SINGLETON_DEFINITION(Scale9Builder);

Scale9Builder::Scale9Builder()
{
}

Scale9Builder::~Scale9Builder()
{
	Clear();
}

void Scale9Builder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::map<const escale9::Symbol*, const PackScale9*>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackScale9*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void Scale9Builder::Clear()
{
	std::map<const escale9::Symbol*, const PackScale9*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* Scale9Builder::Create(const escale9::Symbol* sym)
{
	std::map<const escale9::Symbol*, const PackScale9*>::iterator
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	PackScale9* node = new PackScale9(sym);
	sym->AddReference();
	m_map_data.insert(std::make_pair(sym, node));
	node->AddReference();
	return node;
}

}