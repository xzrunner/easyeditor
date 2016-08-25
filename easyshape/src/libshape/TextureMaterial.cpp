#include "TextureMaterial.h"

#include <ee/ImageSymbol.h>
#include <ee/Image.h>
#include <ee/FileHelper.h>

namespace eshape
{

TextureMaterial::TextureMaterial()
	: m_image(NULL)
{
}

TextureMaterial::TextureMaterial(ee::ImageSymbol* image)
	: m_image(NULL)
	, s2::TexturePolygon(image->GetImage()->GetS2Tex())
{
	cu::RefCountObjAssign(m_image, image);
}

TextureMaterial::~TextureMaterial()
{
	if (m_image) {
		m_image->RemoveReference();
	}
}

Json::Value TextureMaterial::Store(const std::string& dirpath) const
{
	Json::Value val;
	val["type"] = "texture";
	val["texture path"] = ee::FileHelper::GetRelativePath(dirpath, m_image->GetFilepath());
	return val;
}

void TextureMaterial::Translate(const sm::vec2& offset)
{
	Material::Translate(offset);
	for (int i = 0, n = m_texcoords.size(); i < n; ++i) {
		m_texcoords[i] += offset;
	}
}

}