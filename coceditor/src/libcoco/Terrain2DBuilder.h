#ifndef _LIBCOCO_TERRAIN_BUILDER_H_
#define _LIBCOCO_TERRAIN_BUILDER_H_

#include "INodeBuilder.h"

#include <easyterrain2d.h>

namespace libcoco
{

class IPackNode;
class PackAnimation;

class Terrain2DBuilder : public INodeBuilder
{
public:
	Terrain2DBuilder();
	virtual ~Terrain2DBuilder();

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	const IPackNode* Create(const eterrain2d::Symbol* symbol);

private:
	void Load(const eterrain2d::Symbol* symbol, PackAnimation* anim);

private:
	std::map<const eterrain2d::Symbol*, const PackAnimation*> m_map_data;

}; // Terrain2DBuilder

}

#endif // _LIBCOCO_TERRAIN_BUILDER_H_
