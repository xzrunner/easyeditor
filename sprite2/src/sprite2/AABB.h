#ifndef _SPRITE2_AABB_H_
#define _SPRITE2_AABB_H_

namespace s2
{

class AABB
{ 
public:
	AABB();

private:
	int m_xmin, m_ymin;
	int m_xmax, m_ymax;

}; // AABB

}

#include "AABB.inl"

#endif // _SPRITE2_AABB_H_