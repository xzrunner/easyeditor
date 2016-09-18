#ifndef _EASYSPRPACKER_ANIMATION_BUILDER_H_
#define _EASYSPRPACKER_ANIMATION_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace eanim { class Symbol; }

namespace esprpacker
{

class PackAnimation;

class AnimationBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const eanim::Symbol* sym);

private:
	std::map<const eanim::Symbol*, const PackAnimation*> m_map_data;

	SINGLETON_DECLARATION(AnimationBuilder);

}; // AnimationBuilder

}

#endif // _EASYSPRPACKER_ANIMATION_BUILDER_H_