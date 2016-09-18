#include "AnimationBuilder.h"
#include "PackAnimation.h"
#include "ExportNameSet.h"

#include <easyanim.h>

namespace esprpacker
{

SINGLETON_DEFINITION(AnimationBuilder);

AnimationBuilder::AnimationBuilder()
{
}

AnimationBuilder::~AnimationBuilder()
{
	Clear();
}

void AnimationBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::map<const eanim::Symbol*, const PackAnimation*>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackAnimation*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void AnimationBuilder::Clear()
{
	std::map<const eanim::Symbol*, const PackAnimation*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* AnimationBuilder::Create(const eanim::Symbol* sym)
{
	std::map<const eanim::Symbol*, const PackAnimation*>::iterator
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	PackAnimation* node = new PackAnimation(sym);
	ExportNameSet::Instance()->Insert(sym, node);
	sym->AddReference();
	m_map_data.insert(std::make_pair(sym, node));
	node->AddReference();
	return node;
}

}