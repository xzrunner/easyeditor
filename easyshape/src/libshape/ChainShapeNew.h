//#ifndef _EASYSHAPE_CHAIN_SHAPE_H_
//#define _EASYSHAPE_CHAIN_SHAPE_H_
//
//#include <ee/Shape.h>
//
//#include <sprite2/PolylineShape.h>
//
//namespace eshape
//{
//
//class ChainShape : public ee::Shape
//{
//public:
//	ChainShape();
//	ChainShape(const std::vector<sm::vec2>& vertices, bool closed);
//
//	//
//	// IObject interface
//	//
//	virtual ChainShape* Clone() const;
//
//	//
//	// Shape interface
//	//
//	virtual const char* GetShapeDesc() const { return "chain"; }
//	virtual bool IsContain(const sm::vec2& pos) const { return m_core.IsContain(pos); }
//	virtual bool IsIntersect(const sm::rect& rect) const { return m_core.IsIntersect(rect); }
//	virtual void Translate(const sm::vec2& offset);
//	virtual const sm::rect& GetRect() const { return m_core.GetRegion(); }
//	virtual void Draw(const sm::mat4& mt,
//		const s2::RenderColor& color = s2::RenderColor()) const;
//	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
//	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
//	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;
//
//	virtual void Add(size_t index, const sm::vec2& pos);
//	// todo: too slow! 
//	// cache index or pointer? how to avoid iterator's invalid
//	// maybe use list instead of array, but others' effective? 
//	virtual void Remove(const sm::vec2& pos);
//	virtual void Change(const sm::vec2& from, const sm::vec2& to);
//	virtual void Load(const std::vector<sm::vec2>& vertices);
//	virtual void refresh() {}
//
//	void SetClosed(bool closed) { m_core.SetClosed(closed); }
//	bool IsClosed() const { return m_core.IsClosed(); }
//
//	const std::vector<sm::vec2>& GetVertices() const { return m_core.GetVertices(); }
//
//protected:
//	s2::PolylineShape m_core;
//
//	bool m_draw_dir;
//
//}; // ChainShape
//
//}
//
//#endif // _EASYSHAPE_CHAIN_SHAPE_H_