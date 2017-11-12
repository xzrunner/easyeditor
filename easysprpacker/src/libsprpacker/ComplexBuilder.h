#ifndef _EASYSPRPACKER_COMPLEX_BUILDER_H_
#define _EASYSPRPACKER_COMPLEX_BUILDER_H_

#include "NodeBuilder.h"

#include <cu/cu_macro.h>

#include <map>

namespace ecomplex { class Symbol; }

namespace esprpacker
{

class PackComplex;

class ComplexBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const override;

	virtual void Clear() override;

	const PackNode* Create(const std::shared_ptr<const ecomplex::Symbol>& sym);

private:
	std::map<std::shared_ptr<const ecomplex::Symbol>, const PackComplex*> m_map_data;

	CU_SINGLETON_DECLARATION(ComplexBuilder);

}; // ComplexBuilder

}

#endif // _EASYSPRPACKER_COMPLEX_BUILDER_H_