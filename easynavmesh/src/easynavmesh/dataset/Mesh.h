#ifndef _EASYNAVMESH_MESH_H_
#define _EASYNAVMESH_MESH_H_

#include <ee/Vector.h>

#include <vector>

namespace ee { class Rect; }

namespace enav
{

class Mesh
{
public:
	Mesh(const ee::Rect& region);

	void AddHole(const std::vector<ee::Vector>& hole);

	void Draw() const;

private:
	void Build();

private:
	struct Hole
	{
		Hole(const std::vector<ee::Vector>& edge) : edge(edge) {}
		std::vector<ee::Vector> edge;
	};

private:
	std::vector<ee::Vector> m_bound;

	std::vector<Hole> m_holes;

	std::vector<ee::Vector> m_triangles;

}; // Mesh

}

#endif // _EASYNAVMESH_MESH_H_