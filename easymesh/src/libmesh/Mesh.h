#ifndef _EASYMESH_MESH_H_
#define _EASYMESH_MESH_H_

#include <json/json.h>

#include <ee/Object.h>
#include <ee/Cloneable.h>
#include <ee/Color.h>

namespace ee { class Symbol; class Vector; class Rect; class RenderParams; }

namespace emesh
{

class Node;
class Triangle;

class Mesh : public ee::Object, public ee::Cloneable
{
public:
	Mesh();
	Mesh(const Mesh& mesh);
	Mesh(const ee::Symbol* base);
	virtual ~Mesh();

 	//
 	// Cloneable interface
 	//
 	virtual Mesh* Clone() const { return NULL; }

	virtual void Load(const Json::Value& value) {}
	virtual void Store(Json::Value& value) const {}

	virtual void OffsetUV(float dx, float dy) {}
	virtual void Update() {}
	virtual void Refresh() {}

	Node* PointQueryNode(const ee::Vector& p);
	void RectQueryNodes(const ee::Rect& r, std::vector<Node*>& nodes);

	const ee::Symbol* GetBaseSymbol() const { return m_base; }

	const std::vector<Triangle*>& GetTriangles() const { return m_tris; }

	float GetNodeRegion() const { return m_node_radius; }

	float GetWidth() const { return m_width; }
	float GetHeight() const { return m_height; }

	ee::Rect GetRegion() const;

protected:
	void ClearTriangles();

	void StoreTriangles(Json::Value& value) const;
	void LoadTriangles(const Json::Value& value);

protected:
	//int m_texid;
	//std::string m_tex_filepath;		// for dtex

	const ee::Symbol* m_base;
	float m_width, m_height;	

	std::vector<Triangle*> m_tris;

	float m_node_radius;

}; // Mesh

}

#endif // _EASYMESH_MESH_H_