#ifndef _EASYMESH_MESH_H_
#define _EASYMESH_MESH_H_

#include <drag2d.h>

namespace emesh
{

class Node;
class Triangle;
class Mesh : public d2d::Object, d2d::ICloneable
{
public:
	Mesh();
	Mesh(const Mesh& mesh);
	Mesh(const d2d::Image& image);
	~Mesh();

	//
	// ICloneable interface
	//
	virtual Mesh* clone() const;

	void insert(const d2d::Vector& p);
	void remove(const d2d::Vector& p);
	d2d::Vector* find(const d2d::Vector& p);
	void move(d2d::Vector* src, const d2d::Vector& dst);

	Node* queryNode(const d2d::Vector& p);

	void drawInfo() const;
	void drawTexture() const;

private:
	void loadTriangles();

	void clearTriangles();

private:
	struct Region
	{
		std::vector<d2d::Vector> bound;
		std::vector<d2d::Vector> nodes;
	};

private:
	int m_texid;

	float m_width, m_height;

//	std::vector<Region> m_regions;
	Region m_region;

	std::vector<Triangle*> m_tris;

}; // Mesh

}

#endif // _EASYMESH_MESH_H_