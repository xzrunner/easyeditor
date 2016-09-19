#include "TrailBuilder.h"
#include "PackTrail.h"
#include "ExportNameSet.h"

#include <easytrail.h>

namespace esprpacker
{

SINGLETON_DEFINITION(TrailBuilder);

TrailBuilder::TrailBuilder()
{
}

TrailBuilder::~TrailBuilder()
{
	Clear();
}

void TrailBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::map<const etrail::Symbol*, const PackTrail*>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackTrail*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void TrailBuilder::Clear()
{
	std::map<const etrail::Symbol*, const PackTrail*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* TrailBuilder::Create(const etrail::Symbol* sym)
{
	std::map<const etrail::Symbol*, const PackTrail*>::iterator
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	PackTrail* node = new PackTrail(sym);
	ExportNameSet::Instance()->Insert(sym, node);
	sym->AddReference();
	m_map_data.insert(std::make_pair(sym, node));
	node->AddReference();
	return node;
}

}