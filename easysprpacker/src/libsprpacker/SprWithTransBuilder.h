#ifndef _EASYSPRPACKER_SPR_WITH_TRANS_BUILDER_H_
#define _EASYSPRPACKER_SPR_WITH_TRANS_BUILDER_H_

#include "NodeBuilder.h"
#include "PackTrans.h"

#include <map>
#include <vector>

namespace esprpacker
{

template <typename TSymbol, typename TSprite, typename TPack>
class SprWithTransBuilder : public NodeBuilder
{
public:
	virtual ~SprWithTransBuilder();

	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const TSprite* spr);

private:
	template <typename TPack>
	struct Item
	{
		PackTrans trans;
		const TPack* node;

		Item() : node(NULL) {}
		Item(const PackTrans& trans, const TPack* node)
			: trans(trans)
			, node(node)
		{}
	}; // Item

private:
	std::map<const TSymbol*, std::vector<Item<TPack> > > m_map_data;

}; // SprWithTransBuilder

}

#include "SprWithTransBuilder.inl"

#endif // _EASYSPRPACKER_SPR_WITH_TRANS_BUILDER_H_