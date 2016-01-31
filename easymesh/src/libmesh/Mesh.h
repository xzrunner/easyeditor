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
	Mesh(const ee::Image& image, bool initBound = true, bool use_region = false);

	//
	// Cloneable interface
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
	virtual void InsertNode(const ee::Vector& p) {}
	virtual void RemoveNode(const ee::Vector& p) {}
	virtual ee::Vector* FindNode(const ee::Vector& p) { return NULL; }
	virtual void MoveNode(ee::Vector* src, const ee::Vector& dst) {}

	virtual void TraverseShape(ee::Visitor& visitor) const;
	virtual bool RemoveShape(ee::Shape* shape);
	virtual bool InsertShape(ee::Shape* shape);
	virtual bool ClearShape();

 	virtual void Reset();
 	virtual void Clear();

	static const char* GetType() { return "mesh"; }

private:
	void RefreshTriangles();

	void GetTriangulation(std::vector<ee::Vector>& tris);
	void LoadFromTriangulation(const std::vector<ee::Vector>& tris);

	void GetRegionBound(std::vector<ee::Vector>& bound) const;

//	void getLinesCutByUVBounds(std::vector<ee::Vector>& lines);

private:
	struct Region
	{
		ee::Rect rect;
		std::vector<libshape::ChainShape*> loops;
	};

private:
	bool m_use_region;

	Region m_region;

	ee::Vector m_uv_offset;

}; // Mesh

}

#endif // _EASYMESH_MESH_H_