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
	PointsMesh(const ee::SymPtr& base);

	/**
	 *  @interface
	 *    Mesh
	 */
	virtual void Load(const Json::Value& value) override;
	virtual void Store(Json::Value& value) const override;
//	virtual void Update() override;
	virtual void Refresh() override;
	virtual void TraverseMesh(ee::RefVisitor<ee::Shape>& visitor) const override;
	virtual bool RemoveMesh(const ee::ShapePtr& mesh) override;
	virtual bool InsertMesh(const ee::ShapePtr& mesh) override;
//	virtual bool ClearMesh() override;
	virtual void Reset() override;
	virtual void Clear() override;

	/**
	 *  @interface
	 *    Shape
	 */
	virtual const char* GetShapeDesc() const override { return "edited_poly"; }
	virtual void Translate(const sm::vec2& offset) override {}
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) override { return NULL; }
// 	virtual void LoadFromFile(const Json::Value& value, const std::string& dir) override;
// 	virtual void StoreToFile(Json::Value& value, const std::string& dir) const override;

	/**
	 *  @interface
	 *    eshape::EditedPolyShape
	 */
	virtual void AddVertex(int index, const sm::vec2& pos) override;
	virtual void RemoveVertex(const sm::vec2& pos) override;
	virtual void ChangeVertex(const sm::vec2& from, const sm::vec2& to) override;
	virtual void SetVertices(const CU_VEC<sm::vec2>& vertices) override;
	virtual const CU_VEC<sm::vec2>& GetVertices() const override { return m_vertices; }
	virtual bool IsClosed() const override { return m_closed; }
	virtual int PointQueryVertex(const sm::vec2& p) const override;
	virtual void RectQueryVertices(const sm::rect& r, std::vector<int>& vertices) const override;

	int  QueryInnerPos(const sm::vec2& pos) const;
	bool InsertInnerPos(const sm::vec2& pos);
	bool RemoveInnerPos(const sm::vec2& pos);
	bool MoveInnerPos(int idx, const sm::vec2& pos);

	void GetInnerPoints(CU_VEC<sm::vec2>& points) const;
	
private:
	void UpdateMeshFromShape();

}; // PointsMesh
	
}

#endif // _EASYMESH_POINTS_MESH_H_