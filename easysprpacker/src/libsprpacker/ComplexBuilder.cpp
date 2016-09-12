#include "ComplexBuilder.h"
#include "PackComplex.h"
#include "ExportNameSet.h"

#include <easycomplex.h>

namespace esprpacker
{

SINGLETON_DEFINITION(ComplexBuilder);

ComplexBuilder::ComplexBuilder()
{
}

ComplexBuilder::~ComplexBuilder()
{
	Clear();
}

void ComplexBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::map<const ecomplex::Symbol*, const PackComplex*>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackComplex*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void ComplexBuilder::Clear()
{
	std::map<const ecomplex::Symbol*, const PackComplex*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* ComplexBuilder::Create(const ecomplex::Symbol* sym)
{
	std::map<const ecomplex::Symbol*, const PackComplex*>::iterator
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	PackComplex* node = new PackComplex(sym);
	ExportNameSet::Instance()->Insert(sym, node);
	sym->AddReference();
	m_map_data.insert(std::make_pair(sym, node));
	node->AddReference();
	return node;
}

}