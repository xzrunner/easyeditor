#ifndef _EASYRESPACKER_TRAIL_BUILDER_H_
#define _EASYRESPACKER_TRAIL_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace etrail { class Sprite; class Symbol; }

namespace erespacker
{

class ExportNameSet;
class IPackNode;
class PackTrail;

class TrailBuilder : public INodeBuilder
{
public:
	TrailBuilder(ExportNameSet& export_set);
	virtual ~TrailBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const;

	const IPackNode* Create(const etrail::Symbol* sym);

private:
	void Load(const etrail::Symbol* sym, PackTrail* trail);

private:
	ExportNameSet& m_export_set;

	std::map<const etrail::Symbol*, const PackTrail*> m_map_data;

}; // TrailBuilder

}

#endif // _EASYRESPACKER_TRAIL_BUILDER_H_