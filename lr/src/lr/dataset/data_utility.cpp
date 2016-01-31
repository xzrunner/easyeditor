#include "data_utility.h"

#include <easytexture.h>
#include <easyshape.h>

namespace lr
{

void create_shapes_from_etxture(etexture::Sprite* spr, 
								std::vector<ee::Shape*>& output)
{
	output.clear();
	const std::vector<ee::Shape*>& shapes = spr->GetSymbol().GetAllShapes();
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		if (libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shapes[i])) {
			std::vector<ee::Vector> bound = poly->GetVertices();
			for (int i = 0, n = bound.size(); i < n; ++i) {
				bound[i] += spr->GetPosition();
			}
			ee::Shape* shape = new libshape::PolygonShape(bound);
			output.push_back(shape);
		}		
	}
}

}