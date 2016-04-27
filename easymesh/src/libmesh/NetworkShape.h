#ifndef _EASYMESH_NETWORK_SHAPE_H_
#define _EASYMESH_NETWORK_SHAPE_H_

#include <easyshape.h>

namespace emesh
{

class NetworkShape : public eshape::ChainShape
{
public:
	
		
private:
	std::vector<ee::Vector> m_inner_vertices;

}; // NetworkShape

}

#endif // _EASYMESH_NETWORK_SHAPE_H_