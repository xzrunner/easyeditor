#ifndef _EASYNAVMESH_MESH_H_
#define _EASYNAVMESH_MESH_H_

#include <SM_Vector.h>

#include <vector>

namespace ee { class Rect; }

namespace enav
{

class Mesh
{
public:
	Mesh(const sm::rect& region);

	void AddHole(const std::vector<sm::vec2>& hole);

	void Draw() const;

private:
	void Build();

private:
	struct Hole
	{
		Hole(const std::vector<sm::vec2>& edge) : edge(edge) {}
		std::vector<sm::vec2> edge;
	};

private:
	std::vector<sm::vec2> m_bound;

	std::vector<Hole> m_holes;

	std::vector<sm::vec2> m_triangles;

}; // Mesh

}

#endif // _EASYNAVMESH_MESH_H_