#ifndef _EASYMESH_MESH_H_
#define _EASYMESH_MESH_H_

#include <ee/Visitor.h>

#include <sprite2/Mesh.h>

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <json/json.h>

namespace ee { class Symbol; class Rect; class RenderParams; class Shape; }

namespace s2 { class MeshNode; class MeshTriangle; }

namespace emesh
{

class Mesh : public virtual s2::Mesh
{
public:
	Mesh();
	Mesh(const Mesh& mesh);
	Mesh(const ee::Symbol* base);

// 	/**
// 	 *  @interface
// 	 *    Cloneable
// 	 */
// 	virtual Mesh* Clone() const { return NULL; }

	/**
	 *  @interface
	 *    Mesh
	 */
	virtual void Load(const Json::Value& value) {}
	virtual void Store(Json::Value& value) const {}
	virtual void Update() {}
	virtual void Refresh() {}

	/**
	 *  @interface
	 *    Editable
	 */
	virtual void TraverseMesh(ee::Visitor<ee::Shape>& visitor) const {}
	virtual bool RemoveMesh(ee::Shape* mesh) { return false; }
	virtual bool InsertMesh(ee::Shape* mesh) { return false; }
	virtual bool ClearMesh() { return false; }
	virtual void Reset() {}
	virtual void Clear() {}

	s2::MeshNode* PointQueryNode(const sm::vec2& p);
	void RectQueryNodes(const sm::rect& r, std::vector<s2::MeshNode*>& nodes);

protected:
	void StoreTriangles(Json::Value& value) const;
	void LoadTriangles(const Json::Value& value);

}; // Mesh

}

#endif // _EASYMESH_MESH_H_