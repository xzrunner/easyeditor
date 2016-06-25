#ifndef _EASYSHAPE_CHAIN_SHAPE_H_
#define _EASYSHAPE_CHAIN_SHAPE_H_

#include "PolylineShape.h"

#include <sprite2/PolylineShape.h>

namespace eshape
{

class ChainShape : public PolylineShape, private s2::PolylineShape
{
public:
	ChainShape();
	ChainShape(const std::vector<sm::vec2>& vertices, bool closed);

	//
	// IObject interface
	//
	virtual ChainShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "chain"; }
	virtual bool IsContain(const sm::vec2& pos) const { return s2::PolylineShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const { return s2::PolylineShape::IsIntersect(rect); }
	virtual void Translate(const sm::vec2& offset);
	virtual const sm::rect& GetRect() const { return s2::PolylineShape::GetBounding(); }
	virtual void Draw(const sm::mat4& mt,
		const s2::RenderColor& color = s2::RenderColor()) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	/*
	 *  interface PolylineShape
	 */
	virtual void AddVertex(int index, const sm::vec2& pos);
	virtual void RemoveVertex(const sm::vec2& pos);
	virtual void ChangeVertex(const sm::vec2& from, const sm::vec2& to);
	virtual void SetVertices(const std::vector<sm::vec2>& vertices);
	virtual const std::vector<sm::vec2>& GetVertices() const { return m_vertices; }
	virtual bool IsClosed() const { return m_closed; }

	void SetClosed(bool Close) { m_closed = Close; }

private:
	bool m_draw_dir;

}; // ChainShape

}

#endif // _EASYSHAPE_CHAIN_SHAPE_H_