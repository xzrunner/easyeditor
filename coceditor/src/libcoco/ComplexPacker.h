#ifndef _LIBCOCO_COMPLEX_PACKER_H_
#define _LIBCOCO_COMPLEX_PACKER_H_

#include <map>

#include <easycomplex.h>

#include "INodePacker.h"

namespace libcoco
{

class IPackNode;
class PackAnimation;
class ExportNameSet;

class ComplexPacker : public INodePacker
{
public:
	ComplexPacker(ExportNameSet& export_set);
	virtual ~ComplexPacker();

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	const IPackNode* Create(const ecomplex::Symbol* symbol);

private:
	void Load(const ecomplex::Symbol* symbol, PackAnimation* anim);

private:
	ExportNameSet& m_export_set;

	std::map<const ecomplex::Symbol*, const PackAnimation*> m_map_data;

}; // ComplexPacker

}

#endif // _LIBCOCO_COMPLEX_PACKER_H_
