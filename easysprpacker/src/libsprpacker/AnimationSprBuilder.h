#ifndef _EASYSPRPACKER_ANIMATION_SPR_BUILDER_H_
#define _EASYSPRPACKER_ANIMATION_SPR_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace eanim { class Symbol; class Sprite; }

namespace esprpacker
{

class PackAnimationSpr;

class AnimationSprBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const eanim::Sprite* spr);

private:
	std::multimap<const eanim::Symbol*, const PackAnimationSpr*> m_map_data;

	SINGLETON_DECLARATION(AnimationSprBuilder);

}; // AnimationSprBuilder

}

#endif // _EASYSPRPACKER_ANIMATION_SPR_BUILDER_H_