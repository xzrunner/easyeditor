#ifndef _GUM_TEXTURE_POLYGON_H_
#define _GUM_TEXTURE_POLYGON_H_

#include <sprite2/TexturePolygon.h>

#include <string>

namespace gum
{

class Image;

class TexturePolygon : public s2::TexturePolygon
{
public:
	TexturePolygon(const std::string& filepath);
	virtual ~TexturePolygon();

private:
	Image* m_img;

}; // TexturePolygon

}

#endif // _GUM_TEXTURE_POLYGON_H_