#ifndef _EASYSHAPE_TEXTURE_MATERIAL_H_
#define _EASYSHAPE_TEXTURE_MATERIAL_H_

#include "Material.h"

#include <sprite2/TexturePolygon.h>

namespace ee { class ImageSymbol; }

namespace eshape
{

class TextureMaterial : public Material, public s2::TexturePolygon
{
public:
	TextureMaterial();
	TextureMaterial(ee::ImageSymbol* image);
	virtual ~TextureMaterial();

	virtual Json::Value Store(const std::string& dirpath) const;
	virtual void Translate(const sm::vec2& offset);

	const ee::ImageSymbol* GetImage() const { return m_image; }

private:
	ee::ImageSymbol* m_image;

}; // TextureMaterial

}

#endif // _EASYSHAPE_TEXTURE_MATERIAL_H_