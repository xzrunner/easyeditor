#ifndef _EASYSHAPE_TEXTURE_MATERIAL_H_
#define _EASYSHAPE_TEXTURE_MATERIAL_H_

#include "Material.h"

#include <ee/Rect.h>

namespace ee { class ImageSymbol; }

namespace eshape
{

class TextureMaterial : public Material
{
public:
	TextureMaterial(const std::vector<sm::vec2>& vertices, ee::ImageSymbol* image);
	virtual ~TextureMaterial();

	virtual Json::Value Store(const std::string& dirpath) const;

	virtual void Translate(const sm::vec2& offset);
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor& color) const;	

	virtual void ReloadTexture();

	const std::vector<sm::vec2>& GetTexcoords() const { return m_tris_texcoord; }

	const ee::ImageSymbol* GetImage() const { return m_image; }

protected:
	virtual void BuildEnd();

private:
	ee::Rect GetBoundingRegion(const std::vector<sm::vec2>& bounding) const;

	void GetTexBoundarySegments(const ee::Rect& rect, std::vector<sm::vec2>& segments);

	void CalTexcoords(const ee::Rect& rect);

private:
	ee::ImageSymbol* m_image;

	std::vector<sm::vec2> m_tris_texcoord;

}; // TextureMaterial

}

#endif // _EASYSHAPE_TEXTURE_MATERIAL_H_