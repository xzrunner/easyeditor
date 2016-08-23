#ifndef _SPRITE2_MESH_H_
#define _SPRITE2_MESH_H_

#include "pre_defined.h"

#include <CU_RefCountObj.h>
#include <CU_Cloneable.h>
#include <SM_Rect.h>

#include <vector>

namespace s2
{

class Symbol;
class MeshTriangle;

class Mesh : public cu::RefCountObj, public cu::Cloneable
{
public:
	Mesh();
	Mesh(const Mesh& mesh);
	Mesh(const Symbol* base);
	virtual ~Mesh();
	
	virtual void OffsetUV(float dx, float dy) {}

	/**
	 *  @interface
	 *    cu::Cloneable
	 *  @note
	 *    should after other virtual
	 */
	virtual Mesh* Clone() const { return NULL; }

	const Symbol* GetBaseSymbol() const { return m_base; }

	float GetWidth() const { return m_width; }
	float GetHeight() const { return m_height; }

	const std::vector<MeshTriangle*>& GetTriangles() const { return m_tris; }

	float GetNodeRegion() const { return m_node_radius; }

	sm::rect GetRegion() const;

protected:
	void ClearTriangles();

protected:
	const Symbol* m_base;
	float m_width, m_height;	

	std::vector<MeshTriangle*> m_tris;

	float m_node_radius;

}; // Mesh

}

#endif // _SPRITE2_MESH_H_