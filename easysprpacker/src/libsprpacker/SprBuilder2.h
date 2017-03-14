#ifndef _EASYSPRPACKER_SPR_BUILDER2_H_
#define _EASYSPRPACKER_SPR_BUILDER2_H_

#include "NodeBuilder.h"

#include <map>

namespace esprpacker
{

template <typename TSprite, typename TPack>
class SprBuilder2 : public NodeBuilder
{
public:
	virtual ~SprBuilder2();

	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const TSprite* spr);

private:
	std::map<const TSprite*, const TPack*> m_map_data;

}; // SprBuilder2

}

#include "SprBuilder2.inl"

#endif // _EASYSPRPACKER_SPR_BUILDER2_H_