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
//	SkeletonMesh(const ee::Symbol* base);
//
//	/**
//	 *  @interface
//	 *    Mesh
//	 */
//	virtual void Load(const Json::Value& value);
//	virtual void Store(Json::Value& value) const;
//	virtual void OffsetUV(float dx, float dy);
//	virtual void Refresh();
//
//	/**
//	 *  @interface
//	 *    Editable
//	 */
//	virtual bool RemoveMesh(ee::Shape* shape);
//	virtual bool InsertMesh(ee::Shape* shape);
//	virtual bool ClearMesh();
// 	virtual void Reset();
// 	virtual void Clear();
//
//	static std::string GetTypeName() { return "skeleton"; }
//
//}; // SkeletonMesh
//
//}
//
//#endif // _EASYMESH_SKELETON_MESH_H_