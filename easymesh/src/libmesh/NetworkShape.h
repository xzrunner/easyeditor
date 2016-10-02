#ifndef _EASYMESH_NETWORK_SHAPE_H_
#define _EASYMESH_NETWORK_SHAPE_H_

#include <easyshape.h>

#include <sprite2/NetworkShape.h>

#include <json/json.h>

namespace emesh
{

class NetworkShape : public eshape::EditedPolyShape, public s2::NetworkShape
{
public:
	NetworkShape() : m_node_radius(0) {}
	NetworkShape(const NetworkShape& nw);
	NetworkShape(const std::vector<sm::vec2>& vertices, float node_radius);

	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual NetworkShape* Clone() const { return new NetworkShape(*this); }

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual const char* GetShapeDesc() const { return "edited_poly"; }
	virtual void Translate(const sm::vec2& offset) {}
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) { return NULL; }

	/**
	 *  @interface
	 *    Mesh
	 */
	virtual void Draw(const sm::mat4& mt,
		const s2::RenderColor& color = s2::RenderColor()) const;
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	/**
	 *  @interface
	 *    EditedPolyShape
	 */
	virtual void AddVertex(int index, const sm::vec2& pos);
	virtual void RemoveVertex(const sm::vec2& pos);
	virtual void ChangeVertex(const sm::vec2& from, const sm::vec2& to);
	virtual void SetVertices(const std::vector<sm::vec2>& vertices);
	virtual const std::vector<sm::vec2>& GetVertices() const { return m_vertices; }
	virtual bool IsClosed() const { return m_closed; }

	bool InsertInner(const sm::vec2& pos);
	bool RemoveInner(const sm::vec2& pos);
	sm::vec2* QueryInner(const sm::vec2& pos);

private:
	float m_node_radius;

}; // NetworkShape

}

#endif // _EASYMESH_NETWORK_SHAPE_H_