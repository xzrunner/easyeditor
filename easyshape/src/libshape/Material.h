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

	virtual void ReloadTexture() = 0;

	const std::vector<d2d::Vector>& GetTriangles() const { return m_tris; }

	void Refresh(const std::vector<d2d::Vector>& vertices);

	void DebugDrawTris(const d2d::Matrix& mt) const;

protected:
	void BuildBegin(const std::vector<d2d::Vector>& outline);
	void BuildSetSegments(const std::vector<d2d::Vector>& segs);
	void BuildSetHoles(const std::vector<std::vector<d2d::Vector> >& holes);
	virtual void BuildEnd() = 0;

protected:
	std::vector<d2d::Vector> m_tris;

	// shape
	std::vector<d2d::Vector> m_outline;
	std::vector<d2d::Vector> m_segments;
	std::vector<std::vector<d2d::Vector> > m_holes;

}; // Material

}

#endif // _LIBSHAPE_INTERFACE_MATERIAL_H_