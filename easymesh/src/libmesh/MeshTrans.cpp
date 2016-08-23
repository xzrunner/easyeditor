#include "MeshTrans.h"
#include "Mesh.h"

#include <ee/JsonSerializer.h>

#include <sprite2/MeshTriangle.h>

#include <set>

namespace emesh
{

void MeshTrans::Load(const Json::Value& value)
{
	m_map.clear();
	std::vector<sm::vec2> from, to;
	ee::JsonSerializer::Load(value["trans"]["from"], from);
	ee::JsonSerializer::Load(value["trans"]["to"], to);
	for (int i = 0, n = from.size(); i < n; ++i) {
		m_map.insert(std::make_pair(from[i], to[i]));
	}
}

void MeshTrans::Store(Json::Value& value) const
{
	Json::Value& trans_val = value["trans"];

	int count = 0;
	std::map<sm::vec2, sm::vec2, sm::Vector2Cmp>::const_iterator itr 
		= m_map.begin();
	for ( ; itr != m_map.end(); ++itr) 
	{
		trans_val["from"][count * 2] = itr->first.x;
		trans_val["from"][count * 2 + 1] = itr->first.y;
		trans_val["to"][count * 2] = itr->second.x;
		trans_val["to"][count * 2 + 1] = itr->second.y;
		++count;
	}
}

}