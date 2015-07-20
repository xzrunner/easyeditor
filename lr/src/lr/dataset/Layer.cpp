#include "Layer.h"

#include <easyshape.h>

namespace lr
{

Layer::Layer()
	: m_editable(true)
	, m_visible(true)
	, m_next_id(0)
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
	d2d::ISprite* spr = static_cast<d2d::ISprite*>(obj);
	m_name_set.erase(spr->name);
	return m_sprites.Remove(spr);
}

bool Layer::InsertSprite(Object* obj)
{
	d2d::ISprite* spr = static_cast<d2d::ISprite*>(obj);
	CheckSpriteName(spr);
	return m_sprites.Insert(spr);
}

bool Layer::ClearSprite()
{
	m_next_id = 0;
	m_name_set.clear();

	return m_sprites.Clear();
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

bool Layer::InsertShape(Object* obj)
{
	return m_shapes.Insert(static_cast<d2d::IShape*>(obj));
}

bool Layer::ClearShape()
{
	return m_shapes.Clear();
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
		if (spr->GetUserData()) {
			continue;
		}

		if (!IsValidFloat(spr->GetPosition().x) || 
			!IsValidFloat(spr->GetPosition().y)) {
			continue;
		}

		Json::Value spr_val;
		spr_val["filepath"] = d2d::FilenameTools::getRelativePath(dir,
			spr->GetSymbol().GetFilepath()).ToStdString();
		spr->Store(spr_val);

		val["sprite"][count++] = spr_val;
	}

	std::vector<d2d::IShape*> shapes;
	m_shapes.Traverse(d2d::FetchAllVisitor<d2d::IShape>(shapes), true);
	count = 0;
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		d2d::IShape* shape = shapes[i];
		if (shape->GetUserData()) {
			continue;
		}
		shape->StoreToFile(val["shape"][count++], dir);
	}
}

bool Layer::Update(int version)
{
	bool ret = false;

	std::vector<d2d::ISprite*> sprites;
	TraverseSprite(d2d::FetchAllVisitor<d2d::ISprite>(sprites), true);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		bool dirty = sprites[i]->Update(version);
		if (dirty) {
			ret = true;
		}
	}

	if (!m_visible) {
		ret = false;
	}
	return ret;
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
		std::string filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
		if (filepath.empty()) {
			std::string filepath = spr_val["filepath"].asString();
			throw d2d::Exception("filepath err: %s", filepath.c_str());
		}
		d2d::ISymbol* symbol = NULL;

		std::string shape_tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape);
		std::string shape_filepath = d2d::FilenameTools::getFilenameAddTag(filepath, shape_tag, "json");
		if (d2d::FilenameTools::isExist(shape_filepath)) {
			symbol = d2d::SymbolMgr::Instance()->FetchSymbol(shape_filepath);
		} else {
			symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
		}

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->Load(spr_val);
		if (!base_path.empty()) {
			UserData* ud = new UserData(base_path);
			sprite->SetUserData(ud);
		}
		CheckSpriteName(sprite);
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
			shape->SetUserData(ud);
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

void Layer::CheckSpriteName(d2d::ISprite* spr)
{
	std::set<std::string>::iterator itr 
		= m_name_set.find(spr->name);
	if (itr != m_name_set.end()) 
	{
		spr->name = "_sprite"+wxString::FromDouble(++m_next_id);
		assert(m_name_set.find(spr->name) == m_name_set.end());
	}
	else
	{
		int pos = spr->name.find("_sprite");
		if (pos != std::string::npos) {
			std::string str = spr->name.substr(pos + 7);
			int num = atoi(str.c_str());
			if (m_next_id < num) {
				m_next_id = num;
			}
		}
	}
	m_name_set.insert(spr->name);
}

}