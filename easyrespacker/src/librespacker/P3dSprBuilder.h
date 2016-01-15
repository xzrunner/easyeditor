#ifndef _LIBRESPACKER_P3D_SPR_BUILDER_H_
#define _LIBRESPACKER_P3D_SPR_BUILDER_H_

#include "INodeBuilder.h"
#include "PackP3dSpr.h"

namespace eparticle3d { class Sprite; }

namespace librespacker
{

class P3dSprBuilder : public INodeBuilder
{
public:
	P3dSprBuilder();
	virtual ~P3dSprBuilder();

	virtual void Traverse(d2d::IVisitor& visitor) const;

	const IPackNode* Create(const eparticle3d::Sprite* spr);

private:
	std::vector<IPackNode*> m_nodes;

}; // P3dSprBuilder

}

#endif // _LIBRESPACKER_P3D_SPR_BUILDER_H_