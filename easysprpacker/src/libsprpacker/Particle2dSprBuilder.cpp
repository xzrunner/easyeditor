#include "Particle2dSprBuilder.h"
#include "PackParticle2dSpr.h"

#include <easyparticle2d.h>

namespace esprpacker
{

SINGLETON_DEFINITION(Particle2dSprBuilder);

Particle2dSprBuilder::Particle2dSprBuilder()
{
}

Particle2dSprBuilder::~Particle2dSprBuilder()
{
	Clear();
}

void Particle2dSprBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::multimap<const eparticle2d::Symbol*, const PackParticle2dSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackParticle2dSpr*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void Particle2dSprBuilder::Clear()
{
	std::multimap<const eparticle2d::Symbol*, const PackParticle2dSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* Particle2dSprBuilder::Create(const eparticle2d::Sprite* spr)
{
	bool loop	= spr->GetLoop();
	bool local	= spr->GetLocalModeDraw();

	const eparticle2d::Symbol* key = dynamic_cast<const eparticle2d::Symbol*>(spr->GetSymbol());
	std::multimap<const eparticle2d::Symbol*, const PackParticle2dSpr*>::const_iterator 
		itr_s = m_map_data.lower_bound(key),
		itr_e = m_map_data.upper_bound(key),
		itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		if (itr->second->IsLoop() == loop &&
			itr->second->IsLocal() == local) {
			itr->second->AddReference();
			return itr->second;
		}
	}

	key->AddReference();
	PackParticle2dSpr* node = new PackParticle2dSpr(spr);
	node->AddReference();
	m_map_data.insert(std::make_pair(key, node));
	return node;
}

}