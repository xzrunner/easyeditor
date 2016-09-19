#ifndef _EASYSPRPACKER_PARTICLE2D_SPR_BUILDER_H_
#define _EASYSPRPACKER_PARTICLE2D_SPR_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace eparticle2d { class Symbol; class Sprite; }

namespace esprpacker
{

class PackParticle2dSpr;

class Particle2dSprBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const eparticle2d::Sprite* spr);

private:
	std::multimap<const eparticle2d::Symbol*, const PackParticle2dSpr*> m_map_data;

	SINGLETON_DECLARATION(Particle2dSprBuilder);

}; // Particle2dSprBuilder

}

#endif // _EASYSPRPACKER_PARTICLE2D_SPR_BUILDER_H_