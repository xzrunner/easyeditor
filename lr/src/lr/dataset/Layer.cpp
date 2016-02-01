#include "Layer.h"
#include "ShapesUD.h"
#include "BaseFileUD.h"
#include "GroupHelper.h"

#include "view/LibraryPanel.h"
#include "view/typedef.h"

#include <easyshape.h>
#include <easycomplex.h>

namespace lr
{

Layer::Layer(int id, LibraryPanel* library)
	: m_id(id)
	, m_library(library)
	, m_editable(true)
	, m_visible(true)
	, m_next_id(0)
{
}

void Layer::TraverseSprite(ee::Visitor& visitor, bool order/* = true*/) const
{
	m_sprites.Traverse(visitor, order);
	m_layer_mgr.TraverseSprite(visitor, ee::DT_ALL, order);
}

void Layer::TraverseSprite(ee::Visitor& visitor, ee::DataTraverseType type, bool order) const
{
	m_sprites.Traverse(visitor, type, order);
	m_layer_mgr.TraverseSprite(visitor, type, order);
}

bool Layer::RemoveSprite(Object* obj)
{
	ee::Sprite* spr = static_cast<ee::Sprite*>(obj);
	m_name_set.erase(spr->name);

	const std::vector<ee::Layer*>& layers = m_layer_mgr.GetAllLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) {
		if (layers[i]->Remove(spr)) {
			return true;
		}
	}
	return m_sprites.Remove(spr);
}

bool Layer::InsertSprite(Object* obj, int idx)
{
	ee::Sprite* spr = static_cast<ee::Sprite*>(obj);
	CheckSpriteName(spr);

	if (m_layer_mgr.selected) {
		return m_layer_mgr.selected->Insert(spr);
	} else {
		return m_sprites.Insert(spr, idx);
	}
}

bool Layer::ClearSprite()
{
	m_next_id = 0;
	m_name_set.clear();

	m_layer_mgr.Clear();

	return m_sprites.Clear();
}

bool Layer::ResetOrderSprite(const Object* obj, bool up)
{
	return m_sprites.ResetOrder(static_cast<const ee::Sprite*>(obj), up);
}

bool Layer::ResetOrderSpriteMost(const Object* obj, bool up)
{
	return m_sprites.ResetOrderMost(static_cast<const ee::Sprite*>(obj), up);
}

void Layer::TraverseShape(ee::Visitor& visitor, bool order) const
{
	m_shapes.Traverse(visitor, order);
	m_layer_mgr.TraverseShape(visitor, order);
}

bool Layer::RemoveShape(Object* obj)
{
	ee::Shape* shape = static_cast<ee::Shape*>(obj);
	if (m_layer_mgr.selected) {
		return m_layer_mgr.selected->Remove(shape);
	} else {
		return m_shapes.Remove(shape);
	}
}

bool Layer::InsertShape(Object* obj)
{
	ee::Shape* shape = static_cast<ee::Shape*>(obj);
	if (m_layer_mgr.selected) {
		return m_layer_mgr.selected->Insert(shape);
	} else {
		return m_shapes.Insert(shape);
	}
}

bool Layer::ClearShape()
{
	m_layer_mgr.Clear();
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

	if (!val["layers"].isNull()) {
		m_layer_mgr.LoadFromFile(val["layers"], dir);
	}
}

void Layer::StoreToFile(Json::Value& val, const std::string& dir) const
{
	val["name"] = m_name;
	val["visible"] = m_visible;
	val["editable"] = m_editable;
	if (!m_base_filepath.empty()) {
		val["base filepath"] = m_base_filepath;
	}

	m_layer_mgr.StoreToFile(val["layers"], dir);

	std::vector<ee::Sprite*> sprites;
	m_sprites.Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites), ee::DT_ALL);
	int count = 0;
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		Json::Value cval;
		if (StoreSprite(sprites[i], cval, dir)) {
			val["sprite"][count++] = cval;
		}
	}

	std::vector<ee::Shape*> shapes;
	m_shapes.Traverse(ee::FetchAllVisitor<ee::Shape>(shapes), true);
	count = 0;
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		ee::Shape* shape = shapes[i];
		if (shape->GetUserData()) {
			UserData* ud = static_cast<UserData*>(shape->GetUserData());
			if (ud->type == UT_BASE_FILE) {
				return;
			}
		}
		shape->StoreToFile(val["shape"][count++], dir);
	}
}

bool Layer::Update(int version)
{
	bool ret = false;

	std::vector<ee::Sprite*> sprites;
	TraverseSprite(ee::FetchAllVisitor<ee::Sprite>(sprites), true);
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

ee::Sprite* Layer::QuerySprite(const std::string& name) const
{
	QueryNameVisitor<ee::Sprite> visitor(name);
	m_sprites.Traverse(visitor, true);
	return visitor.GetResult();
}

ee::Shape* Layer::QueryShape(const std::string& name) const
{
	QueryNameVisitor<ee::Shape> visitor(name);
	m_shapes.Traverse(visitor, true);
	return visitor.GetResult();
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
	while (!spr_val.isNull()) {
		ee::Sprite* spr = LoadSprite(spr_val, dir, base_path);
		m_sprites.Insert(spr);
		spr->Release();
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
		ee::Shape* shape = eshape::ShapeFactory::CreateShapeFromFile(shape_val, dir);
		if (!base_path.empty()) {
			BaseFileUD* ud = new BaseFileUD(base_path);
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

void Layer::CheckSpriteName(ee::Sprite* spr)
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

void Layer::LoadShapesUD(const Json::Value& spr_val, ee::Sprite* spr) const
{
	if (spr_val["ud"].isNull()) {
		return;
	}

	ShapesUD* ud = new ShapesUD;

	const Json::Value& ud_val = spr_val["ud"];
	ud->layer_id = ud_val["layer"].asInt();
	int idx = 0;
	Json::Value shape_val = ud_val["shape"][idx++];
	while (!shape_val.isNull()) {
		ud->shape_names.push_back(shape_val.asString());
		shape_val = ud_val["shape"][idx++];
	}

	spr->SetUserData(ud);
}

void Layer::StoreShapesUD(ee::Sprite* spr, Json::Value& spr_val) const
{
	if (!spr->GetUserData()) {
		return;
	}

	UserData* ud = static_cast<UserData*>(spr->GetUserData());
	if (ud->type == UT_BASE_FILE) {
		return;
	}

	Json::Value val;
	ShapesUD* sud = static_cast<ShapesUD*>(ud);
	val["layer"] = sud->layer_id;
	for (int i = 0, n = sud->shape_names.size(); i < n; ++i) {
		val["shape"][i] = sud->shape_names[i];
	}

	spr_val["ud"] = val;
}

ee::Sprite* Layer::LoadGroup(const Json::Value& val, const std::string& dir, const std::string& base_path)
{
	std::vector<ee::Sprite*> sprites;
	int idx = 0;
	Json::Value cval = val["group"][idx++];
	while (!cval.isNull()) {
		ee::Sprite* spr = LoadSprite(cval, dir, base_path);
		sprites.push_back(spr);
		cval = val["group"][idx++];
	}

	ee::Sprite* group = GroupHelper::Group(sprites);
	group->Load(val);
	for_each(sprites.begin(), sprites.end(), ee::ReleaseObjectFunctor<ee::Sprite>());
	return group;
}

void Layer::StoreGroup(ee::Sprite* spr, Json::Value& val, const std::string& dir) const
{
	val["filepath"] = GROUP_TAG;
	spr->Store(val);

	ecomplex::Symbol* comp = &dynamic_cast<ecomplex::Symbol&>(const_cast<ee::Symbol&>(spr->GetSymbol()));
	assert(comp);
	std::vector<ee::Sprite*>& sprites = comp->m_sprites;
	int count = 0;
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		Json::Value cval;
		if (StoreSprite(sprites[i], cval, dir)) {
			val["group"][count++] = cval;
		}
	}
}

ee::Sprite* Layer::LoadSprite(const Json::Value& val, const std::string& dir, const std::string& base_path)
{
	std::string filepath = val["filepath"].asString();
	if (filepath == GROUP_TAG) {
		return LoadGroup(val, dir, base_path);
	}

	filepath = ee::SymbolSearcher::GetSymbolPath(dir, val);
	if (filepath.empty()) {
		std::string filepath = val["filepath"].asString();
		throw ee::Exception("filepath err: %s", filepath.c_str());
	}
	ee::Symbol* symbol = NULL;

	std::string shape_tag = ee::FileType::GetTag(ee::FileType::e_shape);
	std::string shape_filepath = ee::FileHelper::GetFilenameAddTag(filepath, shape_tag, "json");
	std::string spr_tag;
	if (ee::FileHelper::IsFileExist(shape_filepath)) {
		symbol = ee::SymbolMgr::Instance()->FetchSymbol(shape_filepath);
		const std::vector<ee::Shape*>& shapes = static_cast<eshape::Symbol*>(symbol)->GetShapes();
		if (!shapes.empty()) {
			spr_tag = shapes[0]->name;
		}
	} else {
		symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	}

	if (!symbol) {
		throw ee::Exception("create symbol err: %s", filepath.c_str()); 
	}

	ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
	sprite->Load(val);

	if (!sprite->tag.empty() && sprite->tag[sprite->tag.size()-1] != ';') {
		sprite->tag += ";";
	}
	sprite->tag += spr_tag;

	if (!base_path.empty()) {
		BaseFileUD* ud = new BaseFileUD(base_path);
		sprite->SetUserData(ud);
		sprite->editable = false;
	}
	CheckSpriteName(sprite);

	LoadShapesUD(val, sprite);

	symbol->Release();

	return sprite;
}

bool Layer::StoreSprite(ee::Sprite* spr, Json::Value& val, const std::string& dir) const
{
	if (spr->GetUserData()) {
		UserData* ud = static_cast<UserData*>(spr->GetUserData());
		if (ud->type == UT_BASE_FILE) {
			return false;
		}
	}

	if (!IsValidFloat(spr->GetPosition().x) || 
		!IsValidFloat(spr->GetPosition().y)) {
		return false;
	}

	if (spr->GetSymbol().GetFilepath() == GROUP_TAG) {
		StoreGroup(spr, val, dir);
	} else {
		std::string filepath = spr->GetSymbol().GetFilepath();
		assert(!filepath.empty());
		val["filepath"] = ee::FileHelper::GetRelativePath(dir, filepath).ToStdString();
		spr->Store(val);
		StoreShapesUD(spr, val);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// class Layer::QueryNameVisitor
//////////////////////////////////////////////////////////////////////////

template<typename T>
Layer::QueryNameVisitor<T>::
QueryNameVisitor(const std::string& name)
	: m_name(name)
	, m_result(NULL)
{
}

template<typename T>
void Layer::QueryNameVisitor<T>::
Visit(Object* object, bool& next)
{
	T* t = static_cast<T*>(object);
	if (t->name == m_name) {
		m_result = t;
		next = false;
	} else {
		next = true;
	}
}

}