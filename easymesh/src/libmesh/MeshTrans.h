#ifndef _EASYMESH_MESH_TRANS_H_
#define _EASYMESH_MESH_TRANS_H_

#include <ee/Vector.h>

#include <json/json.h>

namespace emesh
{

class Mesh;

class MeshTrans
{
public:
	void Load(const Json::Value& value);
	void Store(Json::Value& value) const;

	void LoadFromMesh(const Mesh* mesh);
	void StoreToMesh(Mesh* mesh) const;

	void SetTween(const MeshTrans& s, const MeshTrans& e, float process);

private:
	std::map<ee::Vector, ee::Vector, ee::VectorCmp> m_map;

}; // MeshTrans

}

#endif // _EASYMESH_MESH_TRANS_H_