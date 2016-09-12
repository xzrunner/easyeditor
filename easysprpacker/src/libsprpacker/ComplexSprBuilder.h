#ifndef _EASYSPRPACKER_COMPLEX_SPR_BUILDER_H_
#define _EASYSPRPACKER_COMPLEX_SPR_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace ecomplex { class Symbol; class Sprite; }

namespace esprpacker
{

class PackComplexSpr;

class ComplexSprBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const ecomplex::Sprite* spr);

private:
	std::multimap<const ecomplex::Symbol*, const PackComplexSpr*> m_map_data;

	SINGLETON_DECLARATION(ComplexSprBuilder);

}; // ComplexSprBuilder

}

#endif // _EASYSPRPACKER_COMPLEX_SPR_BUILDER_H_