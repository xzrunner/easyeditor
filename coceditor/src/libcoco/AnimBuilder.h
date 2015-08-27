#ifndef _LIBCOCO_ANIM_BUILDER_H_
#define _LIBCOCO_ANIM_BUILDER_H_

#include <easyanim.h>

namespace libcoco
{

class IPackNode;
class PackAnimation;

class AnimBuilder
{
public:
	AnimBuilder();
	~AnimBuilder();

	const IPackNode* Create(const libanim::Symbol* symbol);

private:
	std::map<const libanim::Symbol*, const PackAnimation*> m_map_anim;

}; // AnimBuilder

}

#endif // _LIBCOCO_ANIM_BUILDER_H_