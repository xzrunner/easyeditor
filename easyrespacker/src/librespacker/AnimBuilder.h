#ifndef _EASYRESPACKER_ANIM_BUILDER_H_
#define _EASYRESPACKER_ANIM_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace libanim { class Symbol; }

namespace erespacker
{

class IPackNode;
class PackAnimation;
class ExportNameSet;

class AnimBuilder : public INodeBuilder
{
public:
	AnimBuilder(ExportNameSet& export_set);
	virtual ~AnimBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const;

	const IPackNode* Create(const libanim::Symbol* sym);

private:
	void Load(const libanim::Symbol* sym, PackAnimation* anim);

private:
	ExportNameSet& m_export_set;

	std::map<const libanim::Symbol*, const PackAnimation*> m_map_data;

}; // AnimBuilder

}

#endif // _EASYRESPACKER_ANIM_BUILDER_H_
