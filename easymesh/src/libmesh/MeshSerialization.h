#ifndef _EASYMESH_MESH_SERIALIZATION_H_
#define _EASYMESH_MESH_SERIALIZATION_H_

#include <json/json.h>

namespace s2 { class MeshTransform; }

namespace emesh
{

class MeshSerialization
{
public:
	static void Load(s2::MeshTransform& trans, const Json::Value& value);
	static void Store(const s2::MeshTransform& trans, Json::Value& value);

}; // MeshSerialization.h

}

#endif // _EASYMESH_MESH_SERIALIZATION_H_