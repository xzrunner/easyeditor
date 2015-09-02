#ifndef _LIBCOCO_ETEXTURE_BUILDER_H_
#define _LIBCOCO_ETEXTURE_BUILDER_H_

#include "INodeBuilder.h"

#include <easytexture.h>

namespace libcoco
{

class IPackNode;
class PackPicture;

class TextureBuilder : public INodeBuilder
{
public:
	TextureBuilder();
	virtual ~TextureBuilder();

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	const IPackNode* Create(const etexture::Symbol* symbol);

private:
	void Load(const etexture::Symbol* symbol, PackPicture* pic);

private:
	std::map<const etexture::Symbol*, const PackPicture*> m_map_data;

}; // TextureBuilder

}

#endif // _LIBCOCO_ETEXTURE_BUILDER_H_
