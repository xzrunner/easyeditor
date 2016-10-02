#ifndef _EASYMESH_NETWORK_H_
#define _EASYMESH_NETWORK_H_

#include "Mesh.h"

#include <sprite2/NetworkMesh.h>

namespace eshape { class ChainShape; }

namespace emesh
{

class NetworkShape;

class Network : public Mesh, public s2::NetworkMesh
{
public:
	Network();
	Network(const Network& mesh);
	Network(const ee::Symbol* base);

// 	/**
// 	 *  @interface
// 	 *    Cloneable
// 	 */
// 	virtual Network* Clone() const;

	/**
	 *  @interface
	 *    Mesh
	 */
	virtual void Load(const Json::Value& value);
	virtual void Store(Json::Value& value) const;
	virtual void OffsetUV(float dx, float dy);
	virtual void Refresh();

	/**
	 *  @interface
	 *    Editable
	 */
	virtual void TraverseMesh(ee::Visitor<ee::Shape>& visitor) const;
	virtual bool RemoveMesh(ee::Shape* shape);
	virtual bool InsertMesh(ee::Shape* shape);
	virtual bool ClearMesh();
 	virtual void Reset();
 	virtual void Clear();

	bool InsertInner(const sm::vec2& pos);
	bool RemoveInner(const sm::vec2& pos);
	sm::vec2* QueryInner(const sm::vec2& pos);

	static std::string GetTypeName() { return "network"; }

private:
	const Network& operator = (const Network& nw) {}

private:
//	void getLinesCutByUVBounds(std::vector<sm::vec2>& lines);

private:
	sm::vec2 m_uv_offset;

}; // Network

}

#endif // _EASYMESH_NETWORK_H_