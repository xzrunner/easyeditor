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
#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>
#include <ee/LogMgr.h>
#include <ee/SymbolPath.h>

#include <easyshape.h>
#include <easycomplex.h>

#include <sprite2/SymType.h>
#include <sprite2/RenderParams.h>
#include <sprite2/Sprite.h>
#include <sprite2/UpdateParams.h>
#include <painting2/RenderCamera.h>

namespace lr
{

Layer::Layer(int id, LibraryPanel* library, pt2::CameraMode cam)
	: m_id(id)
	, m_library(library)
	, m_cam_mode(cam)
	, m_editable(true)
	, m_visible(true)
	, m_name_visible(-1)
	, m_next_id(0)
{
}

void Layer::TraverseSprite(ee::RefVisitor<ee::Sprite>& visitor, bool order/* = true*/) const
{
	m_sprs.Traverse(visitor, order);
	m_layer_mgr.TraverseSprite(visitor, ee::DT_ALL, order);
}

void Layer::TraverseSprite(ee::RefVisitor<ee::Sprite>& visitor, ee::DataTraverseType type, bool order) const
{
	m_sprs.Traverse(visitor, type, order);
	m_layer_mgr.TraverseSprite(visitor, type, order);
}

bool Layer::RemoveSprite(const ee::SprPtr& spr)
{
	std::string sname;
	s2::SprNameMap::Instance()->IDToStr(spr->GetName(), sname);
	m_name_set.erase(sname);

	const std::vector<ee::Layer*>& layers = m_layer_mgr.GetAllLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) {
		if (layers[i]->Remove(spr)) {
			return true;
		}
	}
	return m_sprs.Remove(spr);
}

bool Layer::InsertSprite(const ee::SprPtr& spr, int idx)
{
	CheckSpriteName(spr);

	if (spr->GetCamera().GetMode() != m_cam_mode)
	{
		pt2::RenderCamera rc = spr->GetCamera();
		rc.SetMode(m_cam_mode);
		spr->SetCamera(rc);
	}

	if (m_layer_mgr.selected) {
		return m_layer_mgr.selected->Insert(spr);
	} else {
		return m_sprs.Insert(spr, idx);
	}
}

bool Layer::ClearSprite()
{
	m_next_id = 0;
	m_name_set.clear();

	m_layer_mgr.Clear();

	return m_sprs.Clear();
}

bool Layer::ResetOrderSprite(const ee::SprConstPtr& spr, bool up)
{
	return m_sprs.ResetOrder(spr, up);
}

bool Layer::ResetOrderSpriteMost(const ee::SprConstPtr& spr, bool up)
{
	return m_sprs.ResetOrderMost(spr, up);
}

bool Layer::SortSrites(std::vector<ee::SprPtr>& sprs)
{
	return m_sprs.Sort(sprs);
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
	std::vector<ee::SprPtr> sprs;
	m_sprs.Traverse(ee::FetchAllRefVisitor<ee::Sprite>(sprs), ee::DT_ALL);
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		auto& spr = sprs[i];
		spr->SetVisible(true);
		spr->SetEditable(true);
	}
}

void Layer::LoadFromFile(const Json::Value& val, const std::string& dir, int layer_idx)
{
	m_name = val["name"].asString();
	m_editable = val["editable"].asBool();
	m_visible = val["visible"].asBool();
	if (val.isMember("name_visible")) {
		m_name_visible = val["name_visible"].asInt();
	} else {
		m_name_visible = -1;
	}

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
	val["name_visible"] = m_name_visible;
	if (!m_base_filepath.empty()) {
		val["base filepath"] = m_base_filepath;
	}

	m_layer_mgr.StoreToFile(val["layers"], dir);

	std::vector<ee::SprPtr> sprs;
	m_sprs.Traverse(ee::FetchAllRefVisitor<ee::Sprite>(sprs), ee::DT_ALL);
	int count = 0;

	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		auto& spr = sprs[i];

		Json::Value cval;
		if (StoreSprite(spr, cval, dir)) {
			val["sprite"][count++] = cval;
		}

		if (ecomplex::Sprite* complex = std::dynamic_pointer_cast<ecomplex::Sprite>(spr)) {
			const std::shared_ptr<ecomplex::Symbol>& sym = std::dynamic_pointer_cast<const ecomplex::Symbol>>(complex->GetSymbol());
			const std::string& filepath = sym->GetFilepath();
			ecomplex::FileStorer::Store(filepath, sym, ee::FileHelper::GetFileDir(filepath));
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

bool Layer::Update()
{
	bool ret = false;

	std::vector<ee::SprPtr> sprs;
	TraverseSprite(ee::FetchAllRefVisitor<ee::Sprite>(sprs), true);
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		bool dirty = sprs[i]->Update(s2::UpdateParams());
		if (dirty) {
			ret = true;
		}
	}

	if (!m_visible) {
		ret = false;
	}
	return ret;
}

ee::SprPtr Layer::QuerySprite(const std::string& name) const
{
	QueryNameVisitor<ee::Sprite> visitor(name);
	m_sprs.Traverse(visitor, true);
	return visitor.GetResult();
}

ee::Shape* Layer::QueryShape(const std::string& name) const
{
	QueryNameVisitor<ee::Shape> visitor(name);
	m_shapes.Traverse(visitor, true);
	return visitor.GetResult();
}

void Layer::ClearSprCoverInfo()
{
	std::vector<ee::SprPtr> sprs = m_sprs.GetObjs();
	std::vector<ee::SprPtr> covers;
	std::vector<ee::SprPtr>::iterator itr = sprs.begin();
	for ( ; itr != sprs.end(); )
	{
		auto& spr = *itr;
		const std::string& tag = spr->GetTag();
		if (tag.find(COVER_LAYER_TAG) != std::string::npos) 
		{
			std::string tag = spr->GetTag();
			size_t p_begin = tag.find("layer=");
			if (p_begin != std::string::npos) {
				size_t p_end = tag.find(";", p_begin) + 1;
				tag.erase(tag.begin() + p_begin, tag.begin() + p_end);
			}
			covers.push_back(spr);
			spr->SetTag(tag);

			itr = sprs.erase(itr);
		} 
		else 
		{
			++itr;
		}
	}

	class SprCmp
	{
	public:
		bool operator() (const ee::SprPtr s0, const ee::SprPtr s1) const
		{
			return s0->GetPosition().y > s1->GetPosition().y;
		}
	}; // SprCmp
	std::sort(covers.begin(), covers.end(), SprCmp());

	std::copy(covers.begin(), covers.end(), back_inserter(sprs));
	m_sprs.SetObjs(sprs);
}

bool Layer::IsValidFloat(float f)
{
	return (f == f) && (f <= FLT_MAX && f >= -FLT_MAX);
}

void Layer::LoadSprites(const Json::Value& val, const std::string& dir, const std::string& base_path)
{
	int idx = 0;
	Json::Value spr_val = val[idx++];
	while (!spr_val.isNull()) {
		ee::SprPtr spr = nullptr;
		try {
			spr = LoadSprite(spr_val, dir, base_path);
		} catch (ee::Exception& e) {
			ee::LogMgr::Instance()->AddException(e.What());
			spr_val = val[idx++];
			continue;
		}

		if (spr->GetCamera().GetMode() != m_cam_mode)
		{
			pt2::RenderCamera rc = spr->GetCamera();
			rc.SetMode(m_cam_mode);
			spr->SetCamera(rc);
		}

		m_sprs.Insert(spr);
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

void Layer::CheckSpriteName(const ee::SprPtr& spr)
{
	std::string sname;
	s2::SprNameMap::Instance()->IDToStr(spr->GetName(), sname);
	if (sname.empty() || sname[0] != '_') {
		return;
	}

	std::set<std::string>::iterator itr 
		= m_name_set.find(sname);
	if (itr != m_name_set.end()) 
	{
		std::string new_name = "_sprite" + ee::StringHelper::ToString(++m_next_id);
		spr->SetName(new_name);
		assert(m_name_set.find(new_name) == m_name_set.end());
		m_name_set.insert(new_name);
	}
	else
	{
		int pos = sname.find("_sprite");
		if (pos != std::string::npos) {
			std::string str = sname.substr(pos + 7);
			int num = atoi(str.c_str());
			if (m_next_id < num) {
				m_next_id = num;
			}
		}
		m_name_set.insert(sname);
	}
}

void Layer::LoadShapesUD(const Json::Value& spr_val, const ee::SprPtr& spr) const
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

void Layer::StoreShapesUD(const ee::SprPtr& spr, Json::Value& spr_val) const
{
	if (!spr->GetUserData()) {
		return;
	}

	UserData* ud = static_cast<UserData*>(spr->GetUserData());
	if (ud->type != UT_SHAPES) {
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

ee::SprPtr Layer::LoadSprite(const Json::Value& val, const std::string& dir, const std::string& base_path)
{
	std::string filepath = val["filepath"].asString();
	filepath = ee::SymbolSearcher::GetSymbolPath(dir, val);
	if (filepath.empty()) {
		std::string filepath = val["filepath"].asString();
		throw ee::Exception("filepath err: %s", filepath.c_str());
	}
	ee::Symbol* sym = NULL;

	std::string shape_tag = ee::SymbolFile::Instance()->Tag(s2::SYM_SHAPE);
	std::string shape_filepath = ee::FileHelper::GetFilenameAddTag(filepath, shape_tag, "json");
	std::string spr_tag;
	if (ee::FileHelper::IsFileExist(shape_filepath)) {
		sym = ee::SymbolMgr::Instance()->FetchSymbol(shape_filepath);
		const s2::Shape* shape = dynamic_cast<eshape::Symbol*>(sym)->GetShape();
		if (shape) {
			spr_tag = dynamic_cast<eshape::Symbol*>(sym)->GetName();
		}
	} else {
		sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	}

	if (!sym) {
		throw ee::Exception("create symbol err: %s", filepath.c_str()); 
	}

	auto spr = ee::SpriteFactory::Instance()->Create(sym);
	spr->Load(val, dir);

	sym->RefreshThumbnail(sym->GetFilepath());

	std::string tag = spr->GetTag();
	if (!tag.empty() && tag[tag.size()-1] != ';') {
		tag += ";";
	}
	tag += spr_tag;
	spr->SetTag(tag);

	if (!base_path.empty()) {
		BaseFileUD* ud = new BaseFileUD(base_path);
		spr->SetUserData(ud);
		spr->SetEditable(false);
	}
	CheckSpriteName(spr);

	LoadShapesUD(val, spr);

	return spr;
}

bool Layer::StoreSprite(const ee::SprPtr& spr, Json::Value& val, const std::string& dir) const
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

	auto sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());
	val["filepath"] = ee::SymbolPath::GetRelativePath(sym, dir);
	spr->Store(val, dir);
	StoreShapesUD(spr, val);

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