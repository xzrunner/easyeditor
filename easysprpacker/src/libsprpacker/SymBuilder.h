#ifndef _EASYSPRPACKER_SYM_BUILDER_H_
#define _EASYSPRPACKER_SYM_BUILDER_H_

#include "NodeBuilder.h"

#include <map>

namespace esprpacker
{

template <typename TSymbol, typename TPack>
class SymBuilder : public NodeBuilder
{
public:
	SymBuilder(bool export_name = false);
	virtual ~SymBuilder();

	virtual void Traverse(ee::Visitor<PackNode>& visitor) const override;

	virtual void Clear() override;

	const PackNode* Create(const std::shared_ptr<const TSymbol>& sym);

private:
	bool m_export_name;

	std::map<const std::shared_ptr<const TSymbol>, const TPack*> m_map_data;

}; // SymBuilder

}

#include "SymBuilder.inl"

#endif // _EASYSPRPACKER_SYM_BUILDER_H_