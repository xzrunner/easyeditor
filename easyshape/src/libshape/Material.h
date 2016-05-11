#ifndef _EASYSHAPE_INTERFACE_MATERIAL_H_
#define _EASYSHAPE_INTERFACE_MATERIAL_H_

#include <json/json.h>

#include <ee/Object.h>

#include <SM_Matrix.h>

namespace ee { class RenderColor; }
namespace s2 { class RenderColor; }

namespace eshape
{

class Material : public ee::Object
{
public:
	Material() {}
	virtual ~Material() {}

	virtual Json::Value Store(const std::string& dirpath) const = 0;

	virtual void Translate(const sm::vec2& offset);
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor& color) const = 0;

	virtual void ReloadTexture() = 0;

	void BuildBegin(const std::vector<sm::vec2>& outline);
	void BuildSetSegments(const std::vector<sm::vec2>& segs);
	void BuildSetHoles(const std::vector<std::vector<sm::vec2> >& holes);
	virtual void BuildEnd() = 0;

	const std::vector<sm::vec2>& GetTriangles() const { return m_tris; }

	void Refresh(const std::vector<sm::vec2>& vertices);

	void DebugDrawTris(const sm::mat4& mt) const;

protected:
	std::vector<sm::vec2> m_tris;

	// shape
	std::vector<sm::vec2> m_outline;
	std::vector<sm::vec2> m_segments;
	std::vector<std::vector<sm::vec2> > m_holes;

}; // Material

}

#endif // _EASYSHAPE_INTERFACE_MATERIAL_H_