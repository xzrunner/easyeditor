#ifndef _EASYMESH_MESH_TRANS_H_
#define _EASYMESH_MESH_TRANS_H_

#include <sprite2/MeshTransform.h>

#include <json/json.h>

namespace emesh
{

class MeshTrans : public s2::MeshTransform
{
public:
	void Load(const Json::Value& value);
	void Store(Json::Value& value) const;

}; // MeshTrans

}

#endif // _EASYMESH_MESH_TRANS_H_