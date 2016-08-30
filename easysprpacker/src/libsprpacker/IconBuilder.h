#ifndef _EASYSPRPACKER_ICON_BUILDER_H_
#define _EASYSPRPACKER_ICON_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace eicon { class Symbol; }

namespace esprpacker
{

class PackIcon;

class IconBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const eicon::Symbol* sym);

private:
	std::map<const eicon::Symbol*, const PackIcon*> m_map_data;

	SINGLETON_DECLARATION(IconBuilder);

}; // IconBuilder

}

#endif // _EASYSPRPACKER_ICON_BUILDER_H_