#ifndef _EASYMESH_NETWORK_SHAPE_H_
#define _EASYMESH_NETWORK_SHAPE_H_

#include <easyshape.h>

namespace emesh
{

class NetworkShape : public eshape::ChainShape
{
public:
	NetworkShape() : m_node_radius(0) {}
	NetworkShape(const NetworkShape& nw);
	NetworkShape(eshape::ChainShape* loop, float node_radius);

	//
	// IObject interface
	//
	virtual NetworkShape* Clone() const;

	//
	// Mesh interface
	//
	virtual void Draw(const sm::mat4& mt,
		const s2::RenderColor& color = s2::RenderColor()) const;
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	bool InsertInner(const sm::vec2& pos);
	bool RemoveInner(const sm::vec2& pos);
	sm::vec2* QueryInner(const sm::vec2& pos);

	const std::vector<sm::vec2>& GetInnerVertices() const { 
		return m_inner_vertices; 
	}
	void SetInnerVertices(const std::vector<sm::vec2>& inner) { 
		m_inner_vertices = inner;
	}

private:
	std::vector<sm::vec2> m_inner_vertices;

	float m_node_radius;

}; // NetworkShape

}

#endif // _EASYMESH_NETWORK_SHAPE_H_