#include "IconBuilder.h"
#include "PackIcon.h"

#include <easyicon.h>

namespace esprpacker
{

SINGLETON_DEFINITION(IconBuilder);

IconBuilder::IconBuilder()
{
}

IconBuilder::~IconBuilder()
{
	Clear();
}

void IconBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::map<const eicon::Symbol*, const PackIcon*>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackIcon*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void IconBuilder::Clear()
{
	std::map<const eicon::Symbol*, const PackIcon*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* IconBuilder::Create(const eicon::Symbol* sym)
{
	std::map<const eicon::Symbol*, const PackIcon*>::iterator
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	PackIcon* node = new PackIcon(sym);
	sym->AddReference();
	m_map_data.insert(std::make_pair(sym, node));
	node->AddReference();
	return node;
}

}