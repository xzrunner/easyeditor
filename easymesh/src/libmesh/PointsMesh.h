#ifndef _EASYMESH_POINTS_MESH_H_
#define _EASYMESH_POINTS_MESH_H_

#include "Mesh.h"

#include <easyshape.h>

namespace emesh
{

class PointsMeshShape;

class PointsMesh : public Mesh, public eshape::EditedPolyShape, public s2::PolylineShape
{
public:
	PointsMesh();
	PointsMesh(const ee::Symbol* base);

	/**
	 *  @interface
	 *    Mesh
	 */
	virtual void Load(const Json::Value& value);
	virtual void Store(Json::Value& value) const;
//	virtual void Update();
	virtual void Refresh();
	virtual void TraverseMesh(ee::Visitor<ee::Shape>& visitor) const;
	virtual bool RemoveMesh(ee::Shape* mesh);
	virtual bool InsertMesh(ee::Shape* mesh);
//	virtual bool ClearMesh();
	virtual void Reset();
	virtual void Clear();

	/**
	 *  @interface
	 *    Shape
	 */
	virtual const char* GetShapeDesc() const { return "edited_poly"; }
	virtual void Translate(const sm::vec2& offset) {}
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) { return NULL; }
// 	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
// 	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	/**
	 *  @interface
	 *    eshape::EditedPolyShape
	 */
	virtual void AddVertex(int index, const sm::vec2& pos);
	virtual void RemoveVertex(const sm::vec2& pos);
	virtual void ChangeVertex(const sm::vec2& from, const sm::vec2& to);
	virtual void SetVertices(const std::vector<sm::vec2>& vertices);
	virtual const std::vector<sm::vec2>& GetVertices() const { return m_vertices; }
	virtual bool IsClosed() const { return m_closed; }
	virtual int PointQueryVertex(const sm::vec2& p) const;
	virtual void RectQueryVertices(const sm::rect& r, std::vector<int>& vertices) const;

	int  QueryInnerPos(const sm::vec2& pos) const;
	bool InsertInnerPos(const sm::vec2& pos);
	bool RemoveInnerPos(const sm::vec2& pos);
	bool MoveInnerPos(int idx, const sm::vec2& pos);

	void GetInnerPoints(std::vector<sm::vec2>& points) const;
	
private:
	void UpdateMeshFromShape();

}; // PointsMesh
	
}

#endif // _EASYMESH_POINTS_MESH_H_