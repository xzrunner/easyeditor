#ifndef _LIBCOCO_TERRAIN_PACKER_H_
#define _LIBCOCO_TERRAIN_PACKER_H_

#include "INodePacker.h"

#include <easyterrain2d.h>

namespace libcoco
{

class IPackNode;
class PackAnimation;

class Terrain2DPacker : public INodePacker
{
public:
	Terrain2DPacker();
	virtual ~Terrain2DPacker();

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	const IPackNode* Create(const eterrain2d::Symbol* symbol);

private:
	void Load(const eterrain2d::Symbol* symbol, PackAnimation* anim);

private:
	std::map<const eterrain2d::Symbol*, const PackAnimation*> m_map_data;

}; // Terrain2DPacker

}

#endif // _LIBCOCO_TERRAIN_PACKER_H_
