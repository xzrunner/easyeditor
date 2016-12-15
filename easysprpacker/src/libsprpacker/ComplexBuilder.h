#ifndef _EASYSPRPACKER_COMPLEX_BUILDER_H_
#define _EASYSPRPACKER_COMPLEX_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace ecomplex { class Symbol; }

namespace esprpacker
{

class PackComplex;

class ComplexBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const ecomplex::Symbol* sym);

private:
	std::map<const ecomplex::Symbol*, const PackComplex*> m_map_data;

	SINGLETON_DECLARATION(ComplexBuilder);

}; // ComplexBuilder

}

#endif // _EASYSPRPACKER_COMPLEX_BUILDER_H_