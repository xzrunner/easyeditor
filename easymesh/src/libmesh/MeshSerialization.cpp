#include "MeshSerialization.h"

#include <SM_Vector.h>
#include <sprite2/MeshTransform.h>
#include <glue/JsonSerializer.h>

namespace emesh
{

void MeshSerialization::Load(s2::MeshTransform& trans, const Json::Value& value)
{
	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>& map = trans.GetMap();
	map.clear();
	std::vector<sm::vec2> from, to;
	glue::JsonSerializer::Load(value["trans"]["from"], from);
	glue::JsonSerializer::Load(value["trans"]["to"], to);
	for (int i = 0, n = from.size(); i < n; ++i) {
		map.insert(std::make_pair(from[i], to[i]));
	}
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