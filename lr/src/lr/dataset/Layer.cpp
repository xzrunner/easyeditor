#include "Layer.h"

#include <easyshape.h>

namespace lr
{

Layer::Layer()
	: m_editable(true)
	, m_visible(true)
{	
}

Layer::~Layer()
{
}

void Layer::TraverseSprite(d2d::IVisitor& visitor, bool order/* = true*/) const
{
	m_sprites.Traverse(visitor, order);
}

void Layer::TraverseSprite(d2d::IVisitor& visitor, d2d::DataTraverseType type, bool order) const
{
	m_sprites.Traverse(visitor, type, order);
}

bool Layer::RemoveSprite(Object* obj)
{
	return m_sprites.Remove(static_cast<d2d::ISprite*>(obj));
}

void Layer::InsertSprite(Object* obj)
{
	m_sprites.Insert(static_cast<d2d::ISprite*>(obj));
}

void Layer::ClearSprite()
{
	m_sprites.Clear();
}

bool Layer::ResetOrderSprite(const Object* obj, bool up)
{
	return m_sprites.ResetOrder(static_cast<const d2d::ISprite*>(obj), up);
}

void Layer::TraverseShape(d2d::IVisitor& visitor, bool order) const
{
	m_shapes.Traverse(visitor, order);
}

bool Layer::RemoveShape(Object* obj)
{
	return m_shapes.Remove(static_cast<d2d::IShape*>(obj));
}

void Layer::InsertShape(Object* obj)
{
	m_shapes.Insert(static_cast<d2d::IShape*>(obj));
}

void Layer::ClearShape()
{
	m_shapes.Clear();
}

void Layer::LoadFromFile(const Json::Value& val, const std::string& dir)
{
	m_name = val["name"].asString();
	m_editable = val["editable"].asBool();
	m_visible = val["visible"].asBool();

	int idx = 0;
	Json::Value spr_val = val["sprite"][idx++];
	while (!spr_val.isNull()) 
	{
		wxString filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
		d2d::ISymbol* symbol = NULL;
		try {
			wxString shape_tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape);
			wxString shape_filepath = d2d::FilenameTools::getFilenameAddTag(filepath, shape_tag, "json");
			if (d2d::FilenameTools::isExist(shape_filepath)) {
				symbol = d2d::SymbolMgr::Instance()->fetchSymbol(shape_filepath);
			} else {
				symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
			}

			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			sprite->load(spr_val);

			m_sprites.Insert(sprite);

			sprite->Release();
			symbol->Release();
		} catch (d2d::Exception& e) {
			std::cout << "Symbol::loadResources error! File:" << filepath << std::endl;
			std::cout << e.what();
		}

		spr_val = val["sprite"][idx++];
	}

	idx = 0;
	Json::Value shape_val = val["shape"][idx++];
	while (!shape_val.isNull()) 
	{
		d2d::IShape* shape = libshape::ShapeFactory::CreateShapeFromFile(shape_val, dir);
		m_shapes.Insert(shape);
		shape->Release();
		shape_val = val["shape"][idx++];
	}
}

void Layer::StoreToFile(Json::Value& val, const std::string& dir) const
{
	val["name"] = m_name;
	val["visible"] = m_visible;
	val["editable"] = m_editable;

	std::vector<d2d::ISprite*> sprites;
	m_sprites.Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites), d2d::DT_ALL);
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		d2d::ISprite* spr = sprites[i];

		Json::Value spr_val;
		spr_val["filepath"] = d2d::FilenameTools::getRelativePath(dir,
			spr->getSymbol().getFilepath()).ToStdString();
		spr->store(spr_val);

		val["sprite"][i] = spr_val;
	}

	std::vector<d2d::IShape*> shapes;
	m_shapes.Traverse(d2d::FetchAllVisitor<d2d::IShape>(shapes), true);
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		shapes[i]->StoreToFile(val["shape"][i], dir);
	}
}

}