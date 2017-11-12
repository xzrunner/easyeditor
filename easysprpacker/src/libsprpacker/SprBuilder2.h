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

	virtual void Traverse(ee::Visitor<PackNode>& visitor) const override;

	virtual void Clear() override;

	const PackNode* Create(const std::shared_ptr<const TSprite>& spr, bool force_curr = false);

private:
	std::map<const std::shared_ptr<const TSprite>, const TPack*> m_map_data;

}; // SprBuilder2

}

#include "SprBuilder2.inl"

#endif // _EASYSPRPACKER_SPR_BUILDER2_H_