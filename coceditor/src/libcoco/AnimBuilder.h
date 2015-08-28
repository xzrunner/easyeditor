#ifndef _LIBCOCO_ANIM_BUILDER_H_
#define _LIBCOCO_ANIM_BUILDER_H_

#include <easyanim.h>

namespace libcoco
{

class IPackNode;
class PackAnimation;
class ExportNameSet;

class AnimBuilder
{
public:
	AnimBuilder(ExportNameSet& export_set);
	~AnimBuilder();

	const IPackNode* Create(const libanim::Symbol* symbol);

private:
	void Load(const libanim::Symbol* symbol, PackAnimation* anim);

private:
	ExportNameSet& m_export_set;

	std::map<const libanim::Symbol*, const PackAnimation*> m_map_anim;

}; // AnimBuilder

}

#endif // _LIBCOCO_ANIM_BUILDER_H_