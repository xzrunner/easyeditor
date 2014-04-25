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
	Mesh(const d2d::Image& image, bool initBound = true);
	~Mesh();

	//
	// ICloneable interface
	//
	virtual Mesh* clone() const;

	void insert(const d2d::Vector& p);
	void remove(const d2d::Vector& p);
	d2d::Vector* find(const d2d::Vector& p);
	void move(d2d::Vector* src, const d2d::Vector& dst);

	// select
	Node* queryNode(const d2d::Vector& p);
	void queryNode(const d2d::Rect& r, std::vector<Node*>& nodes);

	// draw
	void drawInfoUV() const;
	void drawInfoXY() const;
	void drawTexture() const;

	void tween(const Mesh& begin, const Mesh& end, float process);

	void reset();
	void clear();

	void resetUVOffset(float dx, float dy);

private:
	void refreshTriangles();
	void refreshTrianglesWithUV();

	void clearTriangles();
	void loadTriangles(const std::vector<d2d::Vector>& tris);

	void copyTriangles(const Mesh& mesh);

	void fixNodeToRegion(d2d::Vector& node);

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

	d2d::Vector m_uv_offset;

	friend class FileIO;

}; // Mesh

}

#endif // _EASYMESH_MESH_H_