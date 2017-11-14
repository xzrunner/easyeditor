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
	TextureMaterial(const std::shared_ptr<ee::ImageSymbol>& image);

	virtual Json::Value Store(const std::string& dirpath) const override;
	virtual void Translate(const sm::vec2& offset) override;

	auto& GetImage() const { return m_img; }

}; // TextureMaterial

}

#endif // _EASYSHAPE_TEXTURE_MATERIAL_H_