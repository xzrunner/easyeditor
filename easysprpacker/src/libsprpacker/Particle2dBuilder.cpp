#include "Particle2dBuilder.h"
#include "PackParticle2d.h"
#include "ExportNameSet.h"

#include <easyparticle2d.h>

namespace esprpacker
{

SINGLETON_DEFINITION(Particle2dBuilder);

Particle2dBuilder::Particle2dBuilder()
{
}

Particle2dBuilder::~Particle2dBuilder()
{
	Clear();
}

void Particle2dBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::map<const eparticle2d::Symbol*, const PackParticle2d*>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackParticle2d*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void Particle2dBuilder::Clear()
{
	std::map<const eparticle2d::Symbol*, const PackParticle2d*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* Particle2dBuilder::Create(const eparticle2d::Symbol* sym)
{
	std::map<const eparticle2d::Symbol*, const PackParticle2d*>::iterator
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	PackParticle2d* node = new PackParticle2d(sym);
	ExportNameSet::Instance()->Insert(sym, node);
	sym->AddReference();
	m_map_data.insert(std::make_pair(sym, node));
	node->AddReference();
	return node;
}

}