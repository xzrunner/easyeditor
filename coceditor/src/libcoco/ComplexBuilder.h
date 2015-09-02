#ifndef _LIBCOCO_COMPLEX_BUILDER_H_
#define _LIBCOCO_COMPLEX_BUILDER_H_

#include <map>

#include <easycomplex.h>

#include "INodeBuilder.h"

namespace libcoco
{

class IPackNode;
class PackAnimation;
class ExportNameSet;

class ComplexBuilder : public INodeBuilder
{
public:
	ComplexBuilder(ExportNameSet& export_set);
	virtual ~ComplexBuilder();

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	const IPackNode* Create(const ecomplex::Symbol* symbol);

private:
	void Load(const ecomplex::Symbol* symbol, PackAnimation* anim);

private:
	ExportNameSet& m_export_set;

	std::map<const ecomplex::Symbol*, const PackAnimation*> m_map_data;

}; // ComplexBuilder

}

#endif // _LIBCOCO_COMPLEX_BUILDER_H_
