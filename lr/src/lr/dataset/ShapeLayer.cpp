#include "ShapeLayer.h"
#include "ShapesUD.h"
#include "data_utility.h"

#include "view/LibraryPanel.h"

#include <easytexture.h>

namespace lr
{

ShapeLayer::ShapeLayer(LibraryPanel* library)
	: Layer(library)
{
}

bool ShapeLayer::InsertSprite(Object* obj)
{
	d2d::ISprite* spr = static_cast<d2d::ISprite*>(obj);
	etexture::Sprite* tex = dynamic_cast<etexture::Sprite*>(spr);
	if (!tex) {
		return Layer::InsertSprite(obj);
	}

	std::vector<d2d::IShape*> shapes;
	create_shapes_from_etxture(tex, shapes);
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		InsertShape(shapes[i]);
	}

	ShapesUD* ud = new ShapesUD;
	ud->layer = this;
	ud->shapes = shapes;

	d2d::ISprite* base_spr = m_library->GetTerrainLayer()->QuerySprite(spr->name);
	base_spr->SetUserData(ud);	

	return true;
}

}