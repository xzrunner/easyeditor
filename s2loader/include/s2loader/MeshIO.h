#ifndef _S2LOADER_MESH_IO_H_
#define _S2LOADER_MESH_IO_H_

#include <json/json.h>

namespace s2 { class Mesh; }
namespace pm { class MeshTransform; }

namespace s2loader
{

class MeshIO
{
public:
	// MeshTransform
	static void Load(const Json::Value& val, pm::MeshTransform& trans, const s2::Mesh& mesh);
	static void Store(Json::Value& val, const pm::MeshTransform& trans, const s2::Mesh& mesh);

// 	// MeshSkeleton
// 	static void Load(const Json::Value& val, s2::MeshSkeleton& sk);
// 	static void Store(Json::Value& val, const s2::MeshSkeleton& trans);

}; // MeshIO

}

#endif // _S2LOADER_MESH_IO_H_
