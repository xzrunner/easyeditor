#include "ActorInfo.h"

#include <easyshape.h>

using namespace raiden;

ActorInfo::ActorInfo() 
{
	id = -1;
	speed = 100;
	symbol = NULL;
}

void ActorInfo::resetOffset()
{
	if (libshape::Symbol* shape = dynamic_cast<libshape::Symbol*>(const_cast<ee::ISymbol*>(symbol)))
	{
		if (shape->shapes.empty())
			return;

		if (libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(shape->shapes[0]))
		{
			const std::vector<ee::Vector>& vertices = chain->getVertices();
			if (vertices.front().y > vertices.back().y)
				offset = -vertices.front();
			else
				offset = -vertices.back();
		}
	}
}