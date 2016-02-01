#ifndef _EASYSHAPE_TEXTURE_MATERIAL_H_
#define _EASYSHAPE_TEXTURE_MATERIAL_H_

#include "Material.h"

#include <ee/Vector.h>
#include <ee/Rect.h>

namespace ee { class ImageSymbol; }

namespace eshape
{

class TextureMaterial : public Material
{
public:
	TextureMaterial(const std::vector<ee::Vector>& vertices, ee::ImageSymbol* image);
	virtual ~TextureMaterial();

	virtual Json::Value Store(const std::string& dirpath) const;

	virtual void Translate(const ee::Vector& offset);
	virtual void Draw(const ee::Matrix& mt, const ee::ColorTrans& color) const;	

	virtual void ReloadTexture();

	const std::vector<ee::Vector>& GetTexcoords() const { return m_tris_texcoord; }

	const ee::ImageSymbol* GetImage() const { return m_image; }

protected:
	virtual void BuildEnd();

private:
	ee::Rect GetBoundingRegion(const std::vector<ee::Vector>& bounding) const;

	void GetTexBoundarySegments(const ee::Rect& rect, std::vector<ee::Vector>& segments);

	void CalTexcoords(const ee::Rect& rect);

private:
	ee::ImageSymbol* m_image;

	std::vector<ee::Vector> m_tris_texcoord;

}; // TextureMaterial

}

#endif // _EASYSHAPE_TEXTURE_MATERIAL_H_