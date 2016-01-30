#ifndef _LIBRESPACKER_PARTICLE3D_BUILDER_H_
#define _LIBRESPACKER_PARTICLE3D_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace eparticle3d { class Symbol; }

namespace librespacker
{

class IPackNode;
class PackParticle3D;
class P3dSprBuilder;

class Particle3DBuilder : public INodeBuilder
{
public:
	Particle3DBuilder();
	virtual ~Particle3DBuilder();

	virtual void Traverse(d2d::Visitor& visitor) const;

	const IPackNode* Create(const eparticle3d::Symbol* symbol, P3dSprBuilder* spr_builder);

private:
	void Load(const eparticle3d::Symbol* symbol, PackParticle3D* ps);

private:
	std::map<const eparticle3d::Symbol*, const PackParticle3D*> m_map_data;

}; // Particle3DBuilder

}

#endif // _LIBRESPACKER_PARTICLE3D_BUILDER_H_