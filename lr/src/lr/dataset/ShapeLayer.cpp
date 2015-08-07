#include "ShapeLayer.h"

#include <easytexture.h>
#include <easyshape.h>

namespace lr
{

ShapeLayer::ShapeLayer(StagePanel* stage)
	: Layer(stage)
{
}

bool ShapeLayer::InsertSprite(Object* obj)
{
	d2d::ISprite* spr = static_cast<d2d::ISprite*>(obj);
	etexture::Sprite* tex = dynamic_cast<etexture::Sprite*>(spr);
	if (!tex) {
		return Layer::InsertSprite(obj);
	}

	const std::vector<d2d::IShape*>& shapes = tex->GetSymbol().GetAllShapes();
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		if (libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shapes[i])) {
			std::vector<d2d::Vector> bound = poly->GetVertices();
			for (int i = 0, n = bound.size(); i < n; ++i) {
				bound[i] += spr->GetPosition();
			}
			InsertShape(new libshape::PolygonShape(bound));
		}		
	}

	return true;
}

}