#ifndef _SPRITE2_POLYGON_H_
#define _SPRITE2_POLYGON_H_

#include <SM_Vector.h>
#include <SM_Matrix.h>
#include <CU_RefCountObj.h>

#include <vector>

namespace s2
{

class RenderColor;

class Polygon : public cu::RefCountObj
{
public:
	virtual ~Polygon() {}

	virtual void Draw(const sm::mat4& mt, const RenderColor& color) const = 0;
	virtual void Build() = 0;

	const std::vector<sm::vec2>& GetTriangles() const { return m_tris; }

	void SetOutline(const std::vector<sm::vec2>& outline) { m_outline = outline; }
	void SetSegments(const std::vector<sm::vec2>& segments) { m_segments = segments; }
	void SetHoles(const std::vector<std::vector<sm::vec2> >& holes) { m_holes = holes; }	

	void Clear();

	void DebugDraw(const sm::mat4& mt) const;

protected:
	std::vector<sm::vec2> m_tris;

	// shape
	std::vector<sm::vec2> m_outline;
	std::vector<sm::vec2> m_segments;
	std::vector<std::vector<sm::vec2> > m_holes;

}; // Polygon

}

#endif // _SPRITE2_POLYGON_H_