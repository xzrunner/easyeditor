#ifndef _EASYSHAPE_CHAIN_SHAPE_H_
#define _EASYSHAPE_CHAIN_SHAPE_H_

#include <ee/Shape.h>
#include <ee/Rect.h>

namespace eshape
{

class ChainShape : public ee::Shape
{
public:
	ChainShape();
	ChainShape(const ChainShape& chain);
	ChainShape(const std::vector<sm::vec2>& vertices, bool loop);

	//
	// IObject interface
	//
	virtual ChainShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "chain"; }
	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;
	virtual void Translate(const sm::vec2& offset);
	virtual const ee::Rect& GetRect() const { return m_rect; }
	virtual void Draw(const sm::mat4& mt,
		const ee::RenderColor& color = ee::RenderColor()) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	virtual void Add(size_t index, const sm::vec2& pos);
	// todo: too slow! 
	// cache index or pointer? how to avoid iterator's invalid
	// maybe use list instead of array, but others' effective? 
	virtual void Remove(const sm::vec2& pos);
	virtual void Change(const sm::vec2& from, const sm::vec2& to);
	virtual void Load(const std::vector<sm::vec2>& vertices);
	virtual void refresh() {}

	void SetClosed(bool Close) { m_loop = Close; }
	bool IsClosed() const { return m_loop; }

	const std::vector<sm::vec2>& GetVertices() const { return m_vertices; }

protected:
	void InitBounding();

protected:
	std::vector<sm::vec2> m_vertices;
	bool m_loop;

	bool m_draw_dir;

	ee::Rect m_rect;

}; // ChainShape

}

#endif // _EASYSHAPE_CHAIN_SHAPE_H_