#ifndef _EASYSPRPACKER_PARTICLE3D_SPR_BUILDER_H_
#define _EASYSPRPACKER_PARTICLE3D_SPR_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace eparticle3d { class Symbol; class Sprite; }

namespace esprpacker
{

class PackParticle3dSpr;

class Particle3dSprBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const eparticle3d::Sprite* spr);

private:
	std::multimap<const eparticle3d::Symbol*, const PackParticle3dSpr*> m_map_data;

	SINGLETON_DECLARATION(Particle3dSprBuilder);

}; // Particle3dSprBuilder

}

#endif // _EASYSPRPACKER_PARTICLE3D_SPR_BUILDER_H_