#include "TextureMaterial.h"

#include <ee/ImageSymbol.h>
#include <ee/Image.h>
#include <ee/FileHelper.h>

namespace eshape
{

TextureMaterial::TextureMaterial()
{
}

TextureMaterial::TextureMaterial(ee::ImageSymbol* image)
	: s2::TexturePolygon(image)
{
}

Json::Value TextureMaterial::Store(const std::string& dirpath) const
{
	Json::Value val;
	val["type"] = "texture";
	val["texture path"] = ee::FileHelper::GetRelativePath(dirpath, GetImage()->GetFilepath());
	return val;
}

void TextureMaterial::Translate(const sm::vec2& offset)
{
	Material::Translate(offset);
	for (int i = 0, n = m_texcoords.size(); i < n; ++i) {
		m_texcoords[i] += offset;
	}
}

const ee::ImageSymbol* TextureMaterial::GetImage() const 
{ 
	return dynamic_cast<ee::ImageSymbol*>(m_img); 
}

}