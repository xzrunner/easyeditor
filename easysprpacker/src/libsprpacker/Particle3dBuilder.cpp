#include "Particle3dBuilder.h"
#include "PackParticle3d.h"
#include "ExportNameSet.h"

#include <easyparticle3d.h>

namespace esprpacker
{

SINGLETON_DEFINITION(Particle3dBuilder);

Particle3dBuilder::Particle3dBuilder()
{
}

Particle3dBuilder::~Particle3dBuilder()
{
	Clear();
}

void Particle3dBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::map<const eparticle3d::Symbol*, const PackParticle3d*>::const_iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackParticle3d*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void Particle3dBuilder::Clear()
{
	std::map<const eparticle3d::Symbol*, const PackParticle3d*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* Particle3dBuilder::Create(const eparticle3d::Symbol* sym)
{
	std::map<const eparticle3d::Symbol*, const PackParticle3d*>::iterator
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	PackParticle3d* node = new PackParticle3d(sym);
	ExportNameSet::Instance()->Insert(sym, node);
	sym->AddReference();
	m_map_data.insert(std::make_pair(sym, node));
	node->AddReference();
	return node;
}

}