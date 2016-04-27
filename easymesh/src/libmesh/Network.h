#ifndef _EASYMESH_NETWORK_H_
#define _EASYMESH_NETWORK_H_

#include "EditableMesh.h"

#include <ee/Rect.h>
#include <ee/Vector.h>

namespace eshape { class ChainShape; }

namespace emesh
{

class Node;
class Triangle;

class Network : public EditableMesh
{
public:
	Network(bool use_region = false);
	Network(const Network& mesh);
	Network(const ee::Image& image, bool initBound = true, bool use_region = false);

	//
	// Cloneable interface
	//
	virtual Network* Clone() const;

	//
	// Shape interface
	//
	virtual void Load(const Json::Value& value);
	virtual void Store(Json::Value& value) const;

	virtual void OffsetUV(float dx, float dy);
	virtual void Refresh();

	//
	// EditableMesh interface
	//
	virtual void InsertNode(const ee::Vector& p) {}
	virtual void RemoveNode(const ee::Vector& p) {}
	virtual ee::Vector* FindNode(const ee::Vector& p) { return NULL; }
	virtual void MoveNode(ee::Vector* src, const ee::Vector& dst) {}

	virtual void TraverseMesh(ee::Visitor& visitor) const;
	virtual bool RemoveMesh(ee::Shape* shape);
	virtual bool InsertMesh(ee::Shape* shape);
	virtual bool ClearMesh();

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
		std::vector<eshape::ChainShape*> loops;
	};

private:
	bool m_use_region;

	Region m_region;

	ee::Vector m_uv_offset;

}; // Network

}

#endif // _EASYMESH_NETWORK_H_