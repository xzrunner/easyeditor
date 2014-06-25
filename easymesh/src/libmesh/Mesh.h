#ifndef _EASYMESH_MESH_H_
#define _EASYMESH_MESH_H_

#include "Shape.h"

#include <drag2d.h>

namespace emesh
{

class Node;
class Triangle;
class Mesh : public Shape
{
public:
	Mesh();
	Mesh(const Mesh& mesh);
	Mesh(const d2d::Image& image, bool initBound = true);

	//
	// ICloneable interface
	//
	virtual Mesh* clone() const;

	//
	// IShape interface
	//
	virtual void Insert(const d2d::Vector& p);
	virtual void Remove(const d2d::Vector& p);
	virtual d2d::Vector* Find(const d2d::Vector& p);
	virtual void Move(d2d::Vector* src, const d2d::Vector& dst);

//	void tween(const Mesh& begin, const Mesh& end, float process);

	virtual void Reset();
	virtual void Clear();

	virtual void ResetUVOffset(float dx, float dy);

private:
	void getRegionBound(std::vector<d2d::Vector>& bound) const;
	void refreshTriangles();

	void getLinesCutByUVBounds(std::vector<d2d::Vector>& lines);
	void refreshTrianglesWithUV();

	void clearTriangles();
	void loadTriangles(const std::vector<d2d::Vector>& tris);

	void copyTriangles(const Mesh& mesh);

	void absorbNodeToRegion(d2d::Vector& node);
	void removeCornerFromNodes();

private:
	struct Region
	{
		d2d::Rect rect;
//		std::vector<d2d::Vector> bound;
		std::vector<d2d::Vector> nodes;
	};

private:
	Region m_region;

	d2d::Vector m_uv_offset;

	friend class FileIO;

}; // Mesh

}

#endif // _EASYMESH_MESH_H_