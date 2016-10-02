#ifndef _EASYMESH_SKELETON_H_
#define _EASYMESH_SKELETON_H_

#include "Mesh.h"

#include <sprite2/SkeletonMesh.h>

namespace emesh
{

class Skeleton : public Mesh, public s2::SkeletonMesh
{
public:
	Skeleton();
	Skeleton(const Skeleton& mesh);
	Skeleton(const ee::Symbol* base);

	/**
	 *  @interface
	 *    Mesh
	 */
	virtual void Load(const Json::Value& value);
	virtual void Store(Json::Value& value) const;
	virtual void OffsetUV(float dx, float dy);
	virtual void Refresh();

	/**
	 *  @interface
	 *    Editable
	 */
	virtual void TraverseMesh(ee::Visitor<ee::Shape>& visitor) const;
	virtual bool RemoveMesh(ee::Shape* shape);
	virtual bool InsertMesh(ee::Shape* shape);
	virtual bool ClearMesh();
 	virtual void Reset();
 	virtual void Clear();

	static std::string GetTypeName() { return "skeleton"; }

}; // Skeleton

}

#endif // _EASYMESH_SKELETON_H_