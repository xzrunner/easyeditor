#include "MeshSerialization.h"

#include <SM_Vector.h>
#include <sprite2/MeshTransform.h>
#include <gum/JsonSerializer.h>

namespace emesh
{

void MeshSerialization::Load(s2::MeshTransform& trans, const Json::Value& value)
{
	gum::MeshSymLoader::LoadMeshTransform(trans, value);
}

void MeshSerialization::Store(const s2::MeshTransform& trans, Json::Value& value)
{
	Json::Value& trans_val = value["trans"];

	int count = 0;
	const std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>& map = trans.GetMap();
	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr = map.begin();
	for ( ; itr != map.end(); ++itr) 
	{
		trans_val["from"][count * 2] = itr->first.x;
		trans_val["from"][count * 2 + 1] = itr->first.y;
		trans_val["to"][count * 2] = itr->second.x;
		trans_val["to"][count * 2 + 1] = itr->second.y;
		++count;
	}
}

}