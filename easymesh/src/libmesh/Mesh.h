#ifndef _EASYMESH_MESH_H_
#define _EASYMESH_MESH_H_

#include <json/json.h>

#include <SM_Vector.h>
#include <SM_Rect.h>
#include <CU_RefCountObj.h>
#include <CU_Cloneable.h>

namespace ee { class Symbol; class Rect; class RenderParams; }

namespace emesh
{

class Node;
class Triangle;

class Mesh : public cu::RefCountObj, public cu::Cloneable
{
public:
	Mesh();
	Mesh(const Mesh& mesh);
	Mesh(const ee::Symbol* base);
	virtual ~Mesh();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Mesh* Clone() const { return NULL; }

	virtual void Load(const Json::Value& value) {}
	virtual void Store(Json::Value& value) const {}

	virtual void OffsetUV(float dx, float dy) {}
	virtual void Update() {}
	virtual void Refresh() {}

	Node* PointQueryNode(const sm::vec2& p);
	void RectQueryNodes(const sm::rect& r, std::vector<Node*>& nodes);

	const ee::Symbol* GetBaseSymbol() const { return m_base; }

	const std::vector<Triangle*>& GetTriangles() const { return m_tris; }

	float GetNodeRegion() const { return m_node_radius; }

	float GetWidth() const { return m_width; }
	float GetHeight() const { return m_height; }

	sm::rect GetRegion() const;

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