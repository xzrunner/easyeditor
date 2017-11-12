#ifndef _EASYRESPACKER_PARTICLE2D_BUILDER_H_
#define _EASYRESPACKER_PARTICLE2D_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace eparticle2d { class Symbol; }

namespace erespacker
{

class IPackNode;
class PackParticle2D;
class ExportNameSet;

class Particle2DBuilder : public INodeBuilder
{
public:
	Particle2DBuilder(ExportNameSet& export_set);
	virtual ~Particle2DBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const override;

	const IPackNode* Create(const std::shared_ptr<const eparticle2d::Symbol>& sym);

private:
	void Load(const std::shared_ptr<const eparticle2d::Symbol>& sym, PackParticle2D* ps);

private:
	ExportNameSet& m_export_set;

	std::map<std::shared_ptr<const eparticle2d::Symbol>, const PackParticle2D*> m_map_data;

}; // Particle2DBuilder

}

#endif // _EASYRESPACKER_PARTICLE2D_BUILDER_H_