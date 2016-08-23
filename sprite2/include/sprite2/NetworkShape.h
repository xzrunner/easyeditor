#ifndef _SPRITE2_NETWORK_SHAPE_H_
#define _SPRITE2_NETWORK_SHAPE_H_

#include "PolylineShape.h"

namespace s2
{

class NetworkShape : public PolylineShape
{
public:
	NetworkShape();
	NetworkShape(const NetworkShape& nw);
	NetworkShape(const std::vector<sm::vec2>& vertices);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual NetworkShape* Clone() const;	

	const std::vector<sm::vec2>& GetInnerVertices() const { 
		return m_inner_vertices; 
	}
	void SetInnerVertices(const std::vector<sm::vec2>& inner) { 
		m_inner_vertices = inner;
	}

protected:
	std::vector<sm::vec2> m_inner_vertices;

}; // NetworkShape

}

#endif // _SPRITE2_NETWORK_SHAPE_H_