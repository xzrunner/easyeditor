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

bool ShapeLayer::InsertSprite(const ee::SprPtr& spr)
{
	etexture::Sprite* tex = std::dynamic_pointer_cast<etexture::Sprite>(spr);
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

	std::string sname;
	s2::SprNameMap::Instance()->IDToStr(spr->GetName(), sname);
	ee::SprPtr base_spr = m_library->GetTerrainLayer()->QuerySprite(sname);
	base_spr->SetUserData(ud);	

	return true;
}

}