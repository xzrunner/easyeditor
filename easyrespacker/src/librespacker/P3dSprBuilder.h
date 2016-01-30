#ifndef _LIBRESPACKER_P3D_SPR_BUILDER_H_
#define _LIBRESPACKER_P3D_SPR_BUILDER_H_

#include "INodeBuilder.h"
#include "PackP3dSpr.h"

namespace eparticle3d { class Sprite; class Symbol; }

namespace librespacker
{

class ExportNameSet;

class P3dSprBuilder : public INodeBuilder
{
public:
	P3dSprBuilder(ExportNameSet& export_set);
	virtual ~P3dSprBuilder();

	virtual void Traverse(d2d::Visitor& visitor) const;

	const IPackNode* Create(const eparticle3d::Sprite* spr);
	void Create(const eparticle3d::Symbol* sym, const IPackNode* p3d);

private:
	ExportNameSet& m_export_set;

	std::vector<IPackNode*> m_nodes;

}; // P3dSprBuilder

}

#endif // _LIBRESPACKER_P3D_SPR_BUILDER_H_