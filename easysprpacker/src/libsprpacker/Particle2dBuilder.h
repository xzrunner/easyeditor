#ifndef _EASYSPRPACKER_PARTICLE2D_BUILDER_H_
#define _EASYSPRPACKER_PARTICLE2D_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace eparticle2d { class Symbol; }

namespace esprpacker
{

class PackParticle2d;

class Particle2dBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const eparticle2d::Symbol* sym);

private:
	std::map<const eparticle2d::Symbol*, const PackParticle2d*> m_map_data;

	SINGLETON_DECLARATION(Particle2dBuilder);

}; // Particle2dBuilder

}

#endif // _EASYSPRPACKER_PARTICLE2D_BUILDER_H_