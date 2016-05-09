#ifndef _EASYMESH_NETWORK_H_
#define _EASYMESH_NETWORK_H_

#include "EditableMesh.h"

#include <ee/Rect.h>

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
	Network(const ee::Symbol* base);
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

	bool InsertInner(const sm::vec2& pos);
	bool RemoveInner(const sm::vec2& pos);
	sm::vec2* QueryInner(const sm::vec2& pos);

	static const char* GetType() { return "network"; }

private:
	const Network& operator = (const Network& nw) {}

private:
	void RefreshTriangles();

	void GetTriangulation(std::vector<sm::vec2>& tris);
	void LoadFromTriangulation(const std::vector<sm::vec2>& tris);

	void GetRegionBound(std::vector<sm::vec2>& bound) const;

//	void getLinesCutByUVBounds(std::vector<sm::vec2>& lines);

private:
	NetworkShape* m_nw;

	sm::vec2 m_uv_offset;

}; // Network

}

#endif // _EASYMESH_NETWORK_H_