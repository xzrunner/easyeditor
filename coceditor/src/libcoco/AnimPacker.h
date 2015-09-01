#ifndef _LIBCOCO_ANIM_PACKER_H_
#define _LIBCOCO_ANIM_PACKER_H_

#include "INodePacker.h"

#include <easyanim.h>

namespace libcoco
{

class IPackNode;
class PackAnimation;
class ExportNameSet;

class AnimPacker : public INodePacker
{
public:
	AnimPacker(ExportNameSet& export_set);
	virtual ~AnimPacker();

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen, 
		const TexturePacker& tp) const;

	const IPackNode* Create(const libanim::Symbol* symbol);

private:
	void Load(const libanim::Symbol* symbol, PackAnimation* anim);

private:
	ExportNameSet& m_export_set;

	std::map<const libanim::Symbol*, const PackAnimation*> m_map_data;

}; // AnimPacker

}

#endif // _LIBCOCO_ANIM_PACKER_H_
