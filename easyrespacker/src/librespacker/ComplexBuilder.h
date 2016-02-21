#ifndef _EASYRESPACKER_COMPLEX_BUILDER_H_
#define _EASYRESPACKER_COMPLEX_BUILDER_H_

#include "INodeBuilder.h"

#include <vector>
#include <map>

namespace ecomplex { class Symbol; }
namespace ee { class Sprite; }

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

	virtual void Traverse(ee::Visitor& visitor) const;

	const IPackNode* Create(const ecomplex::Symbol* symbol);

private:
	IPackNode* LoadComplex(const ecomplex::Symbol* symbol);

	IPackNode* LoadAnchor(const ecomplex::Symbol* symbol);

	static void GroupFromTag(const std::vector<ee::Sprite*>& src, 
		std::map<std::string, std::vector<ee::Sprite*> >& dst,
		std::vector<ee::Sprite*>& others);

private:
	ExportNameSet& m_export_set;

	ClipboxBuilder* m_cb_builder;

	std::map<const ecomplex::Symbol*, const PackAnimation*> m_map_data;

	std::vector<PackAnimation*> m_gen_nodes;

}; // ComplexBuilder

}

#endif // _EASYRESPACKER_COMPLEX_BUILDER_H_
