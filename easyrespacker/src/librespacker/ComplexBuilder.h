#ifndef _EASYRESPACKER_COMPLEX_BUILDER_H_
#define _EASYRESPACKER_COMPLEX_BUILDER_H_

#include "INodeBuilder.h"

#include <ee/Sprite.h>

#include <sprite2/Sprite.h>

#include <vector>
#include <map>

namespace ecomplex { class Symbol; }

namespace erespacker
{

class IPackNode;
class PackAnimation;
class ExportNameSet;
class ClipboxBuilder;

class ComplexBuilder : public INodeBuilder
{
public:
	ComplexBuilder(ExportNameSet& export_set, ClipboxBuilder* cb_builder);
	virtual ~ComplexBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const;

	const IPackNode* Create(const std::shared_ptr<const ecomplex::Symbol>& sym);

private:
	IPackNode* LoadComplex(const std::shared_ptr<const ecomplex::Symbol>& sym);

	IPackNode* LoadAnchor(const std::shared_ptr<ecomplex::Symbol>& sym);

	static void GroupFromTag(const CU_VEC<s2::SprPtr>& src, 
		std::map<std::string, std::vector<ee::SprPtr> >& dst,
		std::vector<ee::SprPtr>& others);

private:
	ExportNameSet& m_export_set;

	ClipboxBuilder* m_cb_builder;

	std::map<std::shared_ptr<const ecomplex::Symbol>, const PackAnimation*> m_map_data;

	std::vector<PackAnimation*> m_gen_nodes;

}; // ComplexBuilder

}

#endif // _EASYRESPACKER_COMPLEX_BUILDER_H_
