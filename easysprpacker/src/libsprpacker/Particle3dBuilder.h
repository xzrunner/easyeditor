#ifndef _EASYSPRPACKER_PARTICLE3D_BUILDER_H_
#define _EASYSPRPACKER_PARTICLE3D_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace eparticle3d { class Symbol; }

namespace esprpacker
{

class PackParticle3d;

class Particle3dBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const eparticle3d::Symbol* sym);

private:
	std::map<const eparticle3d::Symbol*, const PackParticle3d*> m_map_data;

	SINGLETON_DECLARATION(Particle3dBuilder);

}; // Particle3dBuilder

}

#endif // _EASYSPRPACKER_PARTICLE3D_BUILDER_H_