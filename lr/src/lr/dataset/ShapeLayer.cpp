#include "ShapeLayer.h"
#include "ShapesUD.h"
#include "data_utility.h"

#include "view/LibraryPanel.h"

#include <ee/Shape.h>

#include <easytexture.h>

namespace lr
{

ShapeLayer::ShapeLayer(int id, LibraryPanel* library)
	: Layer(id, library, s2::CM_ORTHO)
{
}

bool ShapeLayer::InsertSprite(ee::Sprite* spr)
{
	etexture::Sprite* tex = dynamic_cast<etexture::Sprite*>(spr);
	if (!tex) {
		return Layer::InsertSprite(spr, -1);
	}

	std::vector<ee::Shape*> shapes;
	create_shapes_from_etxture(tex, shapes);
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		InsertShape(shapes[i]);
	}

	ShapesUD* ud = new ShapesUD;
	ud->layer_id = GetID();
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		ud->shape_names.push_back(shapes[i]->GetName());
	}

	ee::Sprite* base_spr = m_library->GetTerrainLayer()->QuerySprite(spr->GetName());
	base_spr->SetUserData(ud);	

	return true;
}

}