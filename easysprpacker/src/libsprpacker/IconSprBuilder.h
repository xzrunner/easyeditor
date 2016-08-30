#ifndef _EASYSPRPACKER_ICON_SPR_BUILDER_H_
#define _EASYSPRPACKER_ICON_SPR_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace eicon { class Symbol; class Sprite; }

namespace esprpacker
{

class PackIconSpr;

class IconSprBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const eicon::Sprite* spr);

private:
	std::multimap<const eicon::Symbol*, const PackIconSpr*> m_map_data;

	SINGLETON_DECLARATION(IconSprBuilder);

}; // IconSprBuilder

}

#endif // _EASYSPRPACKER_ICON_SPR_BUILDER_H_