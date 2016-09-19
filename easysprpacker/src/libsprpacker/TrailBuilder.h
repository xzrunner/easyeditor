#ifndef _EASYSPRPACKER_TRAIL_BUILDER_H_
#define _EASYSPRPACKER_TRAIL_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace etrail { class Symbol; }

namespace esprpacker
{

class PackTrail;

class TrailBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const etrail::Symbol* sym);

private:
	std::map<const etrail::Symbol*, const PackTrail*> m_map_data;

	SINGLETON_DECLARATION(TrailBuilder);

}; // TrailBuilder

}

#endif // _EASYSPRPACKER_TRAIL_BUILDER_H_