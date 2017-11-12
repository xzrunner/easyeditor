#ifndef _EASYSHAPE_CHAIN_SHAPE_H_
#define _EASYSHAPE_CHAIN_SHAPE_H_

#include "EditedPolyShape.h"

#include <sprite2/PolylineShape.h>

namespace eshape
{

class ChainShape : public EditedPolyShape, public s2::PolylineShape
{
public:
	ChainShape();
	ChainShape(const ChainShape& chain);
	ChainShape(const CU_VEC<sm::vec2>& vertices, bool closed);

	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual ChainShape* Clone() const override { return new ChainShape(*this); }
	virtual bool IsContain(const sm::vec2& pos) const override { return s2::PolylineShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const override { return s2::PolylineShape::IsIntersect(rect); }
	virtual void Draw(const s2::RenderParams& rp) const override;

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual const char* GetShapeDesc() const override { return "chain"; }
	virtual void Translate(const sm::vec2& offset) override;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) override;
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir) override;
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const override;

	/**
	 *  @interface
	 *    EditedPolyShape
	 */
	virtual void AddVertex(int index, const sm::vec2& pos) override;
	virtual void RemoveVertex(const sm::vec2& pos) override;
	virtual void ChangeVertex(const sm::vec2& from, const sm::vec2& to) override;
	virtual void SetVertices(const CU_VEC<sm::vec2>& vertices) override;
	virtual const CU_VEC<sm::vec2>& GetVertices() const override { return m_vertices; }
	virtual bool IsClosed() const override { return m_closed; }

	void SetClosed(bool Close) { m_closed = Close; }

protected:
	virtual void UpdateBounding() override { s2::PolylineShape::UpdateBounding(); }

private:
	bool m_draw_dir;

}; // ChainShape

}

#endif // _EASYSHAPE_CHAIN_SHAPE_H_