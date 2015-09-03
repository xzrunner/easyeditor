#ifndef _LIBCOCO_ANIM_BUILDER_H_
#define _LIBCOCO_ANIM_BUILDER_H_

#include "INodeBuilder.h"

#include <easyanim.h>

namespace libcoco
{

class IPackNode;
class PackAnimation;
class ExportNameSet;

class AnimBuilder : public INodeBuilder
{
public:
	AnimBuilder(ExportNameSet& export_set);
	virtual ~AnimBuilder();

	virtual void Traverse(d2d::IVisitor& visitor) const;

	const IPackNode* Create(const libanim::Symbol* symbol);

private:
	void Load(const libanim::Symbol* symbol, PackAnimation* anim);

private:
	ExportNameSet& m_export_set;

	std::map<const libanim::Symbol*, const PackAnimation*> m_map_data;

}; // AnimBuilder

}

#endif // _LIBCOCO_ANIM_BUILDER_H_
