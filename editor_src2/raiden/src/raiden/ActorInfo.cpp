#include "ActorInfo.h"

using namespace raiden;

ActorInfo::ActorInfo() 
{
	id = -1;
	speed = 100;
	symbol = NULL;
}

void ActorInfo::resetOffset()
{
	if (d2d::EShapeSymbol* shape = dynamic_cast<d2d::EShapeSymbol*>(const_cast<d2d::ISymbol*>(symbol)))
	{
		if (shape->shapes.empty())
			return;

		if (d2d::ChainShape* chain = dynamic_cast<d2d::ChainShape*>(shape->shapes[0]))
		{
			const std::vector<d2d::Vector>& vertices = chain->getVertices();
			if (vertices.front().y > vertices.back().y)
				offset = -vertices.front();
			else
				offset = -vertices.back();
		}
	}
}