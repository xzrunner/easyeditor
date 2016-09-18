#include "Particle3dSprBuilder.h"
#include "PackParticle3dSpr.h"

#include <easyparticle3d.h>

namespace esprpacker
{

SINGLETON_DEFINITION(Particle3dSprBuilder);

Particle3dSprBuilder::Particle3dSprBuilder()
{
}

Particle3dSprBuilder::~Particle3dSprBuilder()
{
	Clear();
}

void Particle3dSprBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	std::multimap<const eparticle3d::Symbol*, const PackParticle3dSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackParticle3dSpr*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void Particle3dSprBuilder::Clear()
{
	std::multimap<const eparticle3d::Symbol*, const PackParticle3dSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* Particle3dSprBuilder::Create(const eparticle3d::Sprite* spr)
{
	bool loop	= spr->IsLoop();
	bool local	= spr->IsLocalModeDraw();
	bool alone	= spr->IsAlone();
	bool reuse	= spr->IsReuse();

	const eparticle3d::Symbol* key = dynamic_cast<const eparticle3d::Symbol*>(spr->GetSymbol());
	std::multimap<const eparticle3d::Symbol*, const PackParticle3dSpr*>::const_iterator 
		itr_s = m_map_data.lower_bound(key),
		itr_e = m_map_data.upper_bound(key),
		itr;
	for (itr = itr_s; itr != itr_e; ++itr) {
		if (itr->second->IsLoop()	== loop &&
			itr->second->IsLocal()	== local &&
			itr->second->IsAlone()	== alone &&
			itr->second->IsReuse()	== reuse) {
			itr->second->AddReference();
			return itr->second;
		}
	}

	key->AddReference();
	PackParticle3dSpr* node = new PackParticle3dSpr(spr);
	node->AddReference();
	m_map_data.insert(std::make_pair(key, node));
	return node;
}

}