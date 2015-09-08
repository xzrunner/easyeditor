#ifndef _LIBCOCO_PARTICLE3D_BUILDER_H_
#define _LIBCOCO_PARTICLE3D_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace eparticle3d { class Symbol; }

namespace libcoco
{

class IPackNode;
class PackParticle3D;

class Particle3DBuilder : public INodeBuilder
{
public:
	Particle3DBuilder();
	virtual ~Particle3DBuilder();

	virtual void Traverse(d2d::IVisitor& visitor) const;

	const IPackNode* Create(const eparticle3d::Symbol* symbol);

private:
	void Load(const eparticle3d::Symbol* symbol, PackParticle3D* ps);

private:
	std::map<const eparticle3d::Symbol*, const PackParticle3D*> m_map_data;

}; // Particle3DBuilder

}

#endif // _LIBCOCO_PARTICLE3D_BUILDER_H_