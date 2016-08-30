#ifndef _EASYSPRPACKER_SCALE9_BUILDER_H_
#define _EASYSPRPACKER_SCALE9_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace escale9 { class Symbol; }

namespace esprpacker
{

class PackScale9;

class Scale9Builder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const escale9::Symbol* sym);

private:
	std::map<const escale9::Symbol*, const PackScale9*> m_map_data;

	SINGLETON_DECLARATION(Scale9Builder);

}; // Scale9Builder

}

#endif // _EASYSPRPACKER_SCALE9_BUILDER_H_