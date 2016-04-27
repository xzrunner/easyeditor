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
	virtual void Draw(const ee::Matrix& mt,
		const ee::RenderColor& color = ee::RenderColor()) const;
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	bool InsertInner(const ee::Vector& pos);
	bool RemoveInner(const ee::Vector& pos);
	ee::Vector* QueryInner(const ee::Vector& pos);

	const std::vector<ee::Vector>& GetInnerVertices() const { 
		return m_inner_vertices; 
	}
	void SetInnerVertices(const std::vector<ee::Vector>& inner) { 
		m_inner_vertices = inner;
	}

private:
	std::vector<ee::Vector> m_inner_vertices;

	float m_node_radius;

}; // NetworkShape

}

#endif // _EASYMESH_NETWORK_SHAPE_H_