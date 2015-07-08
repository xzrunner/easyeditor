#ifndef _EASYMESH_MESH_H_
#define _EASYMESH_MESH_H_

#include "EditShape.h"

namespace libshape { class ChainShape; }

namespace emesh
{

class Node;
class Triangle;

class Mesh : public EditShape
{
public:
	Mesh(bool use_region = false);
	Mesh(const Mesh& mesh);
	Mesh(const d2d::Image& image, bool initBound = true, bool use_region = false);

	//
	// ICloneable interface
	//
	virtual Mesh* Clone() const;

	//
	// Shape interface
	//
	virtual void Load(const Json::Value& value);
	virtual void Store(Json::Value& value) const;

	virtual void OffsetUV(float dx, float dy);
	virtual void Refresh();

	//
	// EditShape interface
	//
	virtual void InsertNode(const d2d::Vector& p) {}
	virtual void RemoveNode(const d2d::Vector& p) {}
	virtual d2d::Vector* FindNode(const d2d::Vector& p) { return NULL; }
	virtual void MoveNode(d2d::Vector* src, const d2d::Vector& dst) {}

	virtual void TraverseShape(d2d::IVisitor& visitor) const;
	virtual bool RemoveShape(d2d::IShape* shape);
	virtual bool InsertShape(d2d::IShape* shape);
	virtual bool ClearShape();

 	virtual void Reset();
 	virtual void Clear();

	static const char* GetType() { return "mesh"; }

private:
	void RefreshTriangles();

	void GetTriangulation(std::vector<d2d::Vector>& tris);
	void LoadFromTriangulation(const std::vector<d2d::Vector>& tris);

	void GetRegionBound(std::vector<d2d::Vector>& bound) const;

//	void getLinesCutByUVBounds(std::vector<d2d::Vector>& lines);

private:
	struct Region
	{
		d2d::Rect rect;
		std::vector<libshape::ChainShape*> loops;
	};

private:
	bool m_use_region;

	Region m_region;

	d2d::Vector m_uv_offset;

}; // Mesh

}

#endif // _EASYMESH_MESH_H_