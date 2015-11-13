#ifndef _LIBRESPACKER_COMPLEX_BUILDER_H_
#define _LIBRESPACKER_COMPLEX_BUILDER_H_

#include "INodeBuilder.h"

#include <drag2d.h>

namespace ecomplex { class Symbol; }

namespace librespacker
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

	virtual void Traverse(d2d::IVisitor& visitor) const;

	const IPackNode* Create(const ecomplex::Symbol* symbol);

private:
	IPackNode* LoadComplex(const ecomplex::Symbol* symbol);

	IPackNode* LoadAnchor(const ecomplex::Symbol* symbol);

	static void GroupFromTag(const std::vector<d2d::ISprite*>& src, 
		std::map<std::string, std::vector<d2d::ISprite*> >& dst,
		std::vector<d2d::ISprite*>& others);

private:
	ExportNameSet& m_export_set;

	ClipboxBuilder* m_cb_builder;

	std::map<const ecomplex::Symbol*, const PackAnimation*> m_map_data;

}; // ComplexBuilder

}

#endif // _LIBRESPACKER_COMPLEX_BUILDER_H_
