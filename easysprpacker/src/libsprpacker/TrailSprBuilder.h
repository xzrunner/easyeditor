#ifndef _EASYSPRPACKER_TRAIL_SPR_BUILDER_H_
#define _EASYSPRPACKER_TRAIL_SPR_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace etrail { class Symbol; class Sprite; }

namespace esprpacker
{

class PackTrailSpr;

class TrailSprBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const etrail::Sprite* spr);

private:
	std::multimap<const etrail::Symbol*, const PackTrailSpr*> m_map_data;

	SINGLETON_DECLARATION(TrailSprBuilder);

}; // TrailSprBuilder

}

#endif // _EASYSPRPACKER_TRAIL_SPR_BUILDER_H_