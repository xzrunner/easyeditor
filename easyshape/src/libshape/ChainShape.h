#ifndef _EASYSHAPE_CHAIN_SHAPE_H_
#define _EASYSHAPE_CHAIN_SHAPE_H_

#include "PolylineShape.h"

#include <sprite2/PolylineShape.h>

namespace eshape
{

class ChainShape : public PolylineShape, public s2::PolylineShape
{
public:
	ChainShape();
	ChainShape(const ChainShape& chain);
	ChainShape(const std::vector<sm::vec2>& vertices, bool closed);

	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual ChainShape* Clone() const { return new ChainShape(*this); }
	virtual bool IsContain(const sm::vec2& pos) const { return s2::PolylineShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const { return s2::PolylineShape::IsIntersect(rect); }
	virtual void Draw(const sm::mat4& mt, 
		const s2::RenderColor& color = s2::RenderColor()) const;

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual const char* GetShapeDesc() const { return "chain"; }
	virtual void Translate(const sm::vec2& offset);
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	/**
	 *  @interface
	 *    PolylineShape
	 */
	virtual void AddVertex(int index, const sm::vec2& pos);
	virtual void RemoveVertex(const sm::vec2& pos);
	virtual void ChangeVertex(const sm::vec2& from, const sm::vec2& to);
	virtual void SetVertices(const std::vector<sm::vec2>& vertices);
	virtual const std::vector<sm::vec2>& GetVertices() const { return m_vertices; }
	virtual bool IsClosed() const { return m_closed; }

	void SetClosed(bool Close) { m_closed = Close; }

protected:
	virtual void UpdateBounding() { s2::PolylineShape::UpdateBounding(); }

private:
	bool m_draw_dir;

}; // ChainShape

}

#endif // _EASYSHAPE_CHAIN_SHAPE_H_