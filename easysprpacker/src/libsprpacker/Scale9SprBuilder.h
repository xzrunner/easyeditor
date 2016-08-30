#ifndef _EASYSPRPACKER_SCALE9_SPR_BUILDER_H_
#define _EASYSPRPACKER_SCALE9_SPR_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <vector>

namespace escale9 { class Sprite; }

namespace esprpacker
{

class PackScale9Spr;

class Scale9SprBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const escale9::Sprite* spr);

private:
	std::vector<const PackScale9Spr*> m_data;

	SINGLETON_DECLARATION(Scale9SprBuilder);

}; // Scale9SprBuilder

}

#endif // _EASYSPRPACKER_SCALE9_SPR_BUILDER_H_