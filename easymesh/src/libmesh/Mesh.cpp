#include "Mesh.h"
#include "color_config.h"

#include <ee/Symbol.h>
#include <ee/std_functor.h>

#include <SM_Test.h>
#include <SM_Calc.h>
#include <sprite2/S2_RVG.h>
#include <gum/JsonSerializer.h>

#include <set>
#include <algorithm>

#include <assert.h>

namespace emesh
{

Mesh::Mesh()
	: s2::Mesh()
{
}

Mesh::Mesh(const ee::Symbol* base)
	: s2::Mesh(base)
{	
}

// void Mesh::StoreTriangles(Json::Value& value) const
// {
// 	std::vector<sm::vec2> transform;
// 	for (int i = 0, n = m_tris.size(); i < n; ++i)
// 	{
// 		s2::MeshTriangle* tri = m_tris[i];
// 		for (int i = 0; i < 3; ++i)
// 			transform.push_back(tri->nodes[i]->xy);
// 	}
// 	gum::JsonSerializer::Store(transform, value);
// }
// 
// void Mesh::LoadTriangles(const Json::Value& value)
// {
// 	std::vector<sm::vec2> transform;
// 	gum::JsonSerializer::Load(value, transform);
// 	int itr = 0;
// 	for (int i = 0, n = m_tris.size(); i < n; ++i)
// 	{
// 		s2::MeshTriangle* tri = m_tris[i];
// 		for (int i = 0; i < 3; ++i)
// 			tri->nodes[i]->xy = transform[itr++];
// 	}
// }

}