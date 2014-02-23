#ifndef _EASYMESH_MESH_H_
#define _EASYMESH_MESH_H_

#include <drag2d.h>

namespace emesh
{

class Triangle;
class Mesh : public d2d::Object
{
public:
	Mesh();
	Mesh(const Mesh& mesh);
	Mesh(float width, float height);
	~Mesh();

	void insert(const d2d::Vector& p);
	void remove(const d2d::Vector& p);

private:
	// load triangles from DT
	void loadTriangles();

	void clearTriangles();

private:
	struct Region
	{
		std::vector<d2d::Vector> bound;
		std::vector<d2d::Vector> nodes;
	};

private:
	float m_width, m_height;

	std::vector<Region> m_regions;

	std::vector<Triangle*> m_tris;

}; // Mesh

}

#endif // _EASYMESH_MESH_H_