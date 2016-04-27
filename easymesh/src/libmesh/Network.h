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
class NetworkShape;

class Network : public EditableMesh
{
public:
	Network();
	Network(const Network& nw);
	Network(const ee::Image& image);
	virtual ~Network();

	//
	// Cloneable interface
	//
	virtual Network* Clone() const;

	//
	// Mesh interface
	//
	virtual void Load(const Json::Value& value);
	virtual void Store(Json::Value& value) const;

	virtual void OffsetUV(float dx, float dy);
	virtual void Refresh();

	virtual void TraverseMesh(ee::Visitor& visitor) const;
	virtual bool RemoveMesh(ee::Shape* shape);
	virtual bool InsertMesh(ee::Shape* shape);
	virtual bool ClearMesh();

 	virtual void Reset();
 	virtual void Clear();

	bool InsertInner(const ee::Vector& pos);
	bool RemoveInner(const ee::Vector& pos);
	ee::Vector* QueryInner(const ee::Vector& pos);

	static const char* GetType() { return "network"; }

private:
	const Network& operator = (const Network& nw) {}

private:
	void RefreshTriangles();

	void GetTriangulation(std::vector<ee::Vector>& tris);
	void LoadFromTriangulation(const std::vector<ee::Vector>& tris);

	void GetRegionBound(std::vector<ee::Vector>& bound) const;

//	void getLinesCutByUVBounds(std::vector<ee::Vector>& lines);

private:
	NetworkShape* m_nw;

	ee::Vector m_uv_offset;

}; // Network

}

#endif // _EASYMESH_NETWORK_H_