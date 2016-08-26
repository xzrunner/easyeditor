#include "data_utility.h"

#include <easytexture.h>
#include <easyshape.h>

namespace lr
{

void create_shapes_from_etxture(etexture::Sprite* spr, 
								std::vector<ee::Shape*>& output)
{
	output.clear();
	const std::vector<s2::PolygonShape*>& polys = dynamic_cast<const etexture::Symbol*>(spr->GetSymbol())->GetPolygons();
	for (int i = 0, n = polys.size(); i < n; ++i) {
		if (eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(polys[i])) {
			std::vector<sm::vec2> bound = poly->GetVertices();
			for (int i = 0, n = bound.size(); i < n; ++i) {
				bound[i] += spr->GetPosition();
			}
			ee::Shape* shape = new eshape::PolygonShape(bound);
			output.push_back(shape);
		}		
	}
}

}