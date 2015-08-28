#ifndef _LIBCOCO_TEXTURE_BUILDER_H_
#define _LIBCOCO_TEXTURE_BUILDER_H_

#include <easytexture.h>

namespace libcoco
{

class IPackNode;
class PackPicture;

class TextureBuilder
{
public:
	TextureBuilder();
	~TextureBuilder();

	const IPackNode* Create(const etexture::Symbol* symbol);

private:
	void Load(const etexture::Symbol* symbol, PackPicture* pic);

private:
	std::map<const etexture::Symbol*, const PackPicture*> m_map_data;

}; // TextureBuilder

}

#endif // _LIBCOCO_TEXTURE_BUILDER_H_