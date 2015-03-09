#ifndef _LIBSHAPE_INTERFACE_MATERIAL_H_
#define _LIBSHAPE_INTERFACE_MATERIAL_H_

#include <drag2d.h>

namespace libshape
{

class Material : public d2d::Object
{
public:
	Material() {}
	virtual ~Material() {}

	virtual Json::Value Store(const std::string& dirpath) const = 0;

	virtual void Translate(const d2d::Vector& offset);
	virtual void Draw(const d2d::Matrix& mt) const = 0;

	void Refresh(const std::vector<d2d::Vector>& vertices);

	void DebugDrawTris(const d2d::Matrix& mt) const;

protected:
	virtual void Build(const std::vector<d2d::Vector>& vertices) = 0;
	virtual void Build(const std::vector<d2d::Vector>& vertices,
		const std::vector<d2d::Vector>& segments) = 0;

protected:
	std::vector<d2d::Vector> m_vertices;

}; // Material

}

#endif // _LIBSHAPE_INTERFACE_MATERIAL_H_