//#ifndef _EASYMESH_SKELETON_MESH_H_
//#define _EASYMESH_SKELETON_MESH_H_
//
//#include "Mesh.h"
//
//#include <sprite2/SkeletonMesh.h>
//
//namespace emesh
//{
//
//class SkeletonMesh : public Mesh, public s2::SkeletonMesh
//{
//public:
//	SkeletonMesh();
//	SkeletonMesh(const SkeletonMesh& mesh);
//	SkeletonMesh(const ee::SymPtr& base);
//
//	/**
//	 *  @interface
//	 *    Mesh
//	 */
//	virtual void Load(const Json::Value& value) override;
//	virtual void Store(Json::Value& value) const override;
//	virtual void OffsetUV(float dx, float dy) override;
//	virtual void Refresh() override;
//
//	/**
//	 *  @interface
//	 *    Editable
//	 */
//	virtual bool RemoveMesh(ee::Shape* shape) override;
//	virtual bool InsertMesh(ee::Shape* shape) override;
//	virtual bool ClearMesh() override;
// 	virtual void Reset() override;
// 	virtual void Clear() override;
//
//	static std::string GetTypeName() { return "skeleton"; }
//
//}; // SkeletonMesh
//
//}
//
//#endif // _EASYMESH_SKELETON_MESH_H_