#ifndef _LIBSHAPE_TEXTURE_MATERIAL_H_
#define _LIBSHAPE_TEXTURE_MATERIAL_H_

#include "Material.h"

namespace libshape
{

class TextureMaterial : public Material
{
public:
	TextureMaterial(const std::vector<d2d::Vector>& vertices, d2d::ImageSymbol* image);
	virtual ~TextureMaterial();

	virtual Json::Value Store(const std::string& dirpath) const;

	virtual void Translate(const d2d::Vector& offset);
	virtual void Draw(const d2d::Matrix& mt) const;	

protected:
	virtual void Build(const std::vector<d2d::Vector>& vertices);
	virtual void Build(const std::vector<d2d::Vector>& vertices,
		const std::vector<d2d::Vector>& segments);

private:
	d2d::Rect GetBoundingRegion(const std::vector<d2d::Vector>& bounding) const;

	void GetTexBoundarySegments(const d2d::Rect& rect, std::vector<d2d::Vector>& segments);

	void CalTexcoords(const d2d::Rect& rect);

private:
	d2d::ImageSymbol* m_image;

	std::vector<d2d::Vector> m_texcoords;

}; // TextureMaterial

}

#endif // _LIBSHAPE_TEXTURE_MATERIAL_H_