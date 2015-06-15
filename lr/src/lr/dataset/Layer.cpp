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

void Layer::LoadFromFile(const Json::Value& val, const std::string& dir, int layer_idx)
{
	m_name = val["name"].asString();
	m_editable = val["editable"].asBool();
	m_visible = val["visible"].asBool();

	if (!val["base filepath"].isNull()) {
		m_base_filepath = val["base filepath"].asString();
		LoadFromBaseFile(layer_idx, m_base_filepath, dir);
	}

	LoadSprites(val["sprite"], dir);
	LoadShapes(val["shape"], dir);
}

void Layer::StoreToFile(Json::Value& val, const std::string& dir) const
{
	val["name"] = m_name;
	val["visible"] = m_visible;
	val["editable"] = m_editable;
	if (!m_base_filepath.empty()) {
		val["base filepath"] = m_base_filepath;
	}

	std::vector<d2d::ISprite*> sprites;
	m_sprites.Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites), d2d::DT_ALL);
	int count = 0;
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		d2d::ISprite* spr = sprites[i];
		if (spr->getUserData()) {
			continue;
		}

		if (!IsValidFloat(spr->getPosition().x) || 
			!IsValidFloat(spr->getPosition().y)) {
			continue;
		}

		Json::Value spr_val;
		spr_val["filepath"] = d2d::FilenameTools::getRelativePath(dir,
			spr->getSymbol().getFilepath()).ToStdString();
		spr->store(spr_val);

		val["sprite"][count++] = spr_val;
	}

	std::vector<d2d::IShape*> shapes;
	m_shapes.Traverse(d2d::FetchAllVisitor<d2d::IShape>(shapes), true);
	count = 0;
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		d2d::IShape* shape = shapes[i];
		if (shape->getUserData()) {
			continue;
		}
		shape->StoreToFile(val["shape"][count++], dir);
	}
}

bool Layer::IsValidFloat(float f)
{
	return (f == f) && (f <= FLT_MAX && f >= -FLT_MAX);
}

void Layer::LoadSprites(const Json::Value& val, const std::string& dir,
						const std::string& base_path)
{
	int idx = 0;
	Json::Value spr_val = val[idx++];
	while (!spr_val.isNull()) 
	{
		wxString filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
		d2d::ISymbol* symbol = NULL;

		wxString shape_tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape);
		wxString shape_filepath = d2d::FilenameTools::getFilenameAddTag(filepath, shape_tag, "json");
		if (d2d::FilenameTools::isExist(shape_filepath)) {
			symbol = d2d::SymbolMgr::Instance()->fetchSymbol(shape_filepath);
		} else {
			symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
		}

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->load(spr_val);
		if (!base_path.empty()) {
			UserData* ud = new UserData(base_path);
			sprite->setUserData(ud);
		}
		m_sprites.Insert(sprite);

		sprite->Release();
		symbol->Release();

		spr_val = val[idx++];
	}
}

void Layer::LoadShapes(const Json::Value& val, const std::string& dir,
					   const std::string& base_path)
{
	int idx = 0;
	Json::Value shape_val = val[idx++];
	while (!shape_val.isNull()) 
	{
		d2d::IShape* shape = libshape::ShapeFactory::CreateShapeFromFile(shape_val, dir);
		if (!base_path.empty()) {
			UserData* ud = new UserData(base_path);
			shape->setUserData(ud);
		}
		m_shapes.Insert(shape);
		shape->Release();
		shape_val = val[idx++];
	}
}

void Layer::LoadFromBaseFile(int layer_idx, const std::string& filepath, const std::string& dir)
{
	std::string filepath_full = dir + "\\" + filepath;

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath_full.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	const Json::Value& layer_val = value["layer"][layer_idx];
	if (!layer_val["base filepath"].isNull()) {
		std::string base_path = layer_val["base filepath"].asString();
		LoadFromBaseFile(layer_idx, base_path, dir);
	}

	LoadSprites(layer_val["sprite"], dir, filepath);
	LoadShapes(layer_val["shape"], dir, filepath);
}

}