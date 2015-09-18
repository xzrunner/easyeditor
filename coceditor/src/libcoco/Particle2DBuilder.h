#ifndef _LIBCOCO_PARTICLE2D_BUILDER_H_
#define _LIBCOCO_PARTICLE2D_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace eparticle2d { class Symbol; }

namespace libcoco
{

class IPackNode;
class PackParticle2D;
class ExportNameSet;

class Particle2DBuilder : public INodeBuilder
{
public:
	Particle2DBuilder(ExportNameSet& export_set);
	virtual ~Particle2DBuilder();

	virtual void Traverse(d2d::IVisitor& visitor) const;

	const IPackNode* Create(const eparticle2d::Symbol* symbol);

private:
	void Load(const eparticle2d::Symbol* symbol, PackParticle2D* ps);

private:
	ExportNameSet& m_export_set;

	std::map<const eparticle2d::Symbol*, const PackParticle2D*> m_map_data;

}; // Particle2DBuilder

}

#endif // _LIBCOCO_PARTICLE2D_BUILDER_H_