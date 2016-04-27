#ifndef _EASYMESH_MESH_H_
#define _EASYMESH_MESH_H_

#include <json/json.h>

#include <ee/Object.h>
#include <ee/Cloneable.h>
#include <ee/Color.h>

namespace ee { class Image; class Vector; class Rect; class Matrix; class RenderParams; }

namespace emesh
{

class Node;
class Triangle;

class Mesh : public ee::Object, public ee::Cloneable
{
public:
	Mesh();
	Mesh(const Mesh& mesh);
	Mesh(const ee::Image& image);
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

	void DrawInfoUV() const;
	void DrawInfoXY() const;
	void DrawTexture(const ee::RenderParams& trans) const;
	void DrawTexture(const ee::RenderParams& trans, unsigned int texid) const;

	const std::vector<Triangle*>& GetTriangles() const { return m_tris; }

	float GetNodeRegion() const { return m_node_radius; }

	ee::Rect GetRegion() const;

	void SetTween(Mesh* begin, Mesh* end, float process);

protected:
	void ClearTriangles();

	void StoreTriangles(Json::Value& value) const;
	void LoadTriangles(const Json::Value& value);

protected:
	int m_texid;
	float m_width, m_height;
	std::string m_tex_filepath;		// for dtex

	std::vector<Triangle*> m_tris;

	float m_node_radius;

}; // Mesh

}

#endif // _EASYMESH_MESH_H_