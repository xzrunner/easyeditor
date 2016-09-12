#include "ComplexSprBuilder.h"
#include "PackComplexSpr.h"

#include <easycomplex.h>

namespace esprpacker
{

SINGLETON_DEFINITION(ComplexSprBuilder);

ComplexSprBuilder::ComplexSprBuilder()
{
}

ComplexSprBuilder::~ComplexSprBuilder()
{
	Clear();
}

void ComplexSprBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::multimap<const ecomplex::Symbol*, const PackComplexSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackComplexSpr*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void ComplexSprBuilder::Clear()
{
	std::multimap<const ecomplex::Symbol*, const PackComplexSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* ComplexSprBuilder::Create(const ecomplex::Sprite* spr)
{
	int action = spr->GetAction();
	const ecomplex::Symbol* key = dynamic_cast<const ecomplex::Symbol*>(spr->GetSymbol());
	std::multimap<const ecomplex::Symbol*, const PackComplexSpr*>::const_iterator 
		itr_s = m_map_data.lower_bound(key),
		itr_e = m_map_data.upper_bound(key),
		itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		if (itr->second->GetAction() == action) {
			itr->second->AddReference();
			return itr->second;
		}
	}

	key->AddReference();
	PackComplexSpr* node = new PackComplexSpr(spr);
	node->AddReference();
	m_map_data.insert(std::make_pair(key, node));
	return node;
}

}