#ifndef _EASYMESH_MESH_H_
#define _EASYMESH_MESH_H_

#include <ee/Visitor.h>

#include <sprite2/S2_Mesh.h>

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <json/json.h>

namespace ee { class Symbol; class Rect; class RenderParams; class Shape; }

namespace emesh
{

class Mesh : public s2::Mesh
{
public:
	Mesh();
	Mesh(const ee::Symbol* base);

	/**
	 *  @interface
	 *    Mesh
	 */
	virtual void Load(const Json::Value& value) {}
	virtual void Store(Json::Value& value) const {}
//	virtual void Update() {}
	virtual void Refresh() {}
	virtual void Draw() const {}
	
	/**
	 *  @interface
	 *    Editable
	 */
	virtual void TraverseMesh(ee::Visitor<ee::Shape>& visitor) const {}
	virtual bool RemoveMesh(ee::Shape* mesh) { return false; }
	virtual bool InsertMesh(ee::Shape* mesh) { return false; }
// 	virtual bool ClearMesh() { return false; }
	virtual void Reset() {}
	virtual void Clear() {}

	virtual int PointQueryVertex(const sm::vec2& p) const = 0;
	virtual void RectQueryVertices(const sm::rect& r, std::vector<int>& vertices) const = 0;

protected:
// 	void StoreTriangles(Json::Value& value) const;
// 	void LoadTriangles(const Json::Value& value);

}; // Mesh

}

#endif // _EASYMESH_MESH_H_