#ifndef _SPRITE2_NETWORK_MESH_H_
#define _SPRITE2_NETWORK_MESH_H_

#include "Mesh.h"

#include <SM_Vector.h>

#include <vector>

namespace s2
{

class Symbol;
class NetworkShape;

class NetworkMesh : public VIRTUAL_INHERITANCE Mesh
{
public:
	NetworkMesh();
	NetworkMesh(const NetworkMesh& nw);
	NetworkMesh(const Symbol* base);
	virtual ~NetworkMesh();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual NetworkMesh* Clone() const { return new NetworkMesh(*this); }
	
protected:
	void RefreshTriangles();

	void GetTriangulation(std::vector<sm::vec2>& tris);
	void LoadFromTriangulation(const std::vector<sm::vec2>& tris);

	//void GetRegionBound(std::vector<sm::vec2>& bound) const;

	VI_DUMMY_FUNC

protected:
	NetworkShape* m_nw;

}; // NetworkMesh

}

#endif // _SPRITE2_NETWORK_MESH_H_