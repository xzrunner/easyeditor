#ifndef _EASYSPRPACKER_SPR_BUILDER_H_
#define _EASYSPRPACKER_SPR_BUILDER_H_

#include "NodeBuilder.h"

#include <map>

namespace esprpacker
{

template <typename TSymbol, typename TSprite, typename TPack>
class SprBuilder : public NodeBuilder
{
public:
	virtual ~SprBuilder();

	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const TSprite* spr);

private:
	std::multimap<const TSymbol*, const TPack*> m_map_data;

}; // SprBuilder

}

#include "SprBuilder.inl"

#endif // _EASYSPRPACKER_SPR_BUILDER_H_