#include "Layer.h"
#include "ShapesUD.h"
#include "BaseFileUD.h"

#include "view/LibraryPanel.h"
#include "view/typedef.h"

#include <ee/Layer.h>
#include <ee/FetchAllVisitor.h>
#include <ee/StringHelper.h>
#include <ee/Exception.h>
#include <ee/FileHelper.h>
#include <ee/SymbolSearcher.h>
#include <ee/SpriteFactory.h>
#include <ee/SymbolMgr.h>

#include <easyshape.h>
#include <easycomplex.h>

#include <sprite2/RenderCamera.h>
#include <sprite2/RenderParams.h>
#include <sprite2/S2_Sprite.h>

namespace lr
{

Layer::Layer(int id, LibraryPanel* library, s2::CameraMode cam)
	: m_id(id)
	, m_library(library)
	, m_cam_mode(cam)
	, m_editable(true)
	, m_visible(true)
	, m_next_id(0)
{
}

void Layer::TraverseSprite(ee::Visitor<ee::Sprite>& visitor, bool order/* = true*/) const
{
	m_sprites.Traverse(visitor, order);
	m_layer_mgr.TraverseSprite(visitor, ee::DT_ALL, order);
}

void Layer::TraverseSprite(ee::Visitor<ee::Sprite>& visitor, ee::DataTraverseType type, bool order) const
{
	m_sprites.Traverse(visitor, type, order);
	m_layer_mgr.TraverseSprite(visitor, type, order);
}

bool Layer::RemoveSprite(ee::Sprite* spr)
{
	m_name_set.erase(spr->GetName());

	const std::vector<ee::Layer*>& layers = m_layer_mgr.GetAllLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) {
		if (layers[i]->Remove(spr)) {
			return true;
		}
	}
	return m_sprites.Remove(spr);
}

bool Layer::InsertSprite(ee::Sprite* spr, int idx)
{
	CheckSpriteName(spr);

	spr->Camera().mode = m_cam_mode;

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

bool Layer::ResetOrderSprite(const ee::Sprite* spr, bool up)
{
	return m_sprites.ResetOrder(spr, up);
}

bool Layer::ResetOrderSpriteMost(const ee::Sprite* spr, bool up)
{
	return m_sprites.ResetOrderMost(spr, up);
}

bool Layer::SortSrites(std::vector<ee::Sprite*>& sprites)
{
	return m_sprites.Sort(sprites);
}

void Layer::TraverseShape(ee::Visitor<ee::Shape>& visitor, bool order) const
{
	m_shapes.Traverse(visitor, order);
	m_layer_mgr.TraverseShape(visitor, order);
}

bool Layer::RemoveShape(ee::Shape* shape)
{
	if (m_layer_mgr.selected) {
		return m_layer_mgr.selected->Remove(shape);
	} else {
		return m_shapes.Remove(shape);
	}
}

bool Layer::InsertShape(ee::Shape* shape)
{
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

bool Layer::IsEditable() const 
{
	if (!m_base_filepath.empty()) {
		return false;
	} else {
		return m_editable; 
	}
}

void Layer::ResetSpritesVisibleEditable()
{
	std::vector<ee::Sprite*> sprites;
	m_sprites.Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites), ee::DT_ALL);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ee::Sprite* spr = sprites[i];
		spr->SetVisible(true);
		spr->SetEditable(true);
	}
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
		ee::Sprite* spr = sprites[i];
		if (spr->GetUserData()) {
			UserData* ud = static_cast<UserData*>(spr->GetUserData());
			if (ud->type == UT_NEW_COMPLEX) {
				const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(spr->GetSymbol());
				ecomplex::FileStorer::Store(sym->GetFilepath().c_str(), 
					static_cast<const ecomplex::Symbol*>(sym));
			}
		}

		Json::Value cval;
		if (StoreSprite(spr, cval, dir)) {
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

bool Layer::Update(float dt)
{
	bool ret = false;

	std::vector<ee::Sprite*> sprites;
	TraverseSprite(ee::FetchAllVisitor<ee::Sprite>(sprites), true);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		bool dirty = sprites[i]->Update(s2::RenderParams(), dt);
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
		spr->Camera().mode = m_cam_mode;
		m_sprites.Insert(spr);
		spr->RemoveReference();
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
		shape->RemoveReference();
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
		= m_name_set.find(spr->GetName());
	if (itr != m_name_set.end()) 
	{
		spr->SetName("_sprite" + ee::StringHelper::ToString(++m_next_id));
		assert(m_name_set.find(spr->GetName()) == m_name_set.end());
	}
	else
	{
		int pos = spr->GetName().find("_sprite");
		if (pos != std::string::npos) {
			std::string str = spr->GetName().substr(pos + 7);
			int num = atoi(str.c_str());
			if (m_next_id < num) {
				m_next_id = num;
			}
		}
	}
	m_name_set.insert(spr->GetName());
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

	std::string name = val["group_name"].asString();

	ecomplex::Sprite* spr = ecomplex::GroupHelper::Group(sprites);
	ecomplex::Symbol* sym = dynamic_cast<ecomplex::Symbol*>(spr->GetSymbol());
	sym->SetFilepath(GROUP_TAG);
	sym->name = name;
	sym->SetName(name);
	spr->SetName(name);

	ee::SpriteFactory::Instance()->Insert(spr);
	spr->Load(val);
	for_each(sprites.begin(), sprites.end(), cu::RemoveRefFonctor<ee::Sprite>());
	return spr;
}

void Layer::StoreGroup(ee::Sprite* spr, Json::Value& val, const std::string& dir) const
{
	val["filepath"] = GROUP_TAG;
	spr->Store(val);

	ecomplex::Symbol* comp = dynamic_cast<ecomplex::Symbol*>(spr->GetSymbol());
	assert(comp);
	const std::vector<s2::Sprite*>& children = comp->GetChildren();
	int count = 0;
	for (int i = 0, n = children.size(); i < n; ++i) {
		Json::Value cval;
		ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
		if (StoreSprite(child, cval, dir)) {
			val["group"][count++] = cval;
		}
	}
	val["group_name"] = comp->GetName();
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
			spr_tag = shapes[0]->GetName();
		}
	} else {
		symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	}

	if (!symbol) {
		throw ee::Exception("create symbol err: %s", filepath.c_str()); 
	}

	ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
	sprite->Load(val);

	std::string tag = sprite->GetTag();
	if (!tag.empty() && tag[tag.size()-1] != ';') {
		tag += ";";
	}
	tag += spr_tag;
	sprite->SetTag(tag);

	if (!base_path.empty()) {
		BaseFileUD* ud = new BaseFileUD(base_path);
		sprite->SetUserData(ud);
		sprite->SetEditable(false);
	}
	CheckSpriteName(sprite);

	LoadShapesUD(val, sprite);

	symbol->RemoveReference();

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

	ee::Symbol* sym = dynamic_cast<ee::Symbol*>(spr->GetSymbol());
	if (sym->GetFilepath() == GROUP_TAG) {
		StoreGroup(spr, val, dir);
	} else {
		std::string filepath = sym->GetFilepath();
		assert(!filepath.empty());
		val["filepath"] = ee::FileHelper::GetRelativePath(dir, filepath);
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
Visit(T* obj, bool& next)
{
	T* t = static_cast<T*>(obj);
	if (t->GetName() == m_name) {
		m_result = t;
		next = false;
	} else {
		next = true;
	}
}

}