#include "PolygonShape.h"
#include "PolygonPropertySetting.h"
#include "ColorMaterial.h"
#include "TextureMaterial.h"

#include <ee/Math2D.h>
#include <ee/SymbolMgr.h>
#include <ee/SettingData.h>
#include <ee/ImageSymbol.h>
#include <ee/Config.h>
#include <ee/color_config.h>

namespace eshape
{

PolygonShape::PolygonShape()
	: m_material(NULL)
{
	m_draw_dir = false;
}

PolygonShape::PolygonShape(const PolygonShape& polygon)
	: ChainShape(polygon)
	, m_material(NULL)
{
	m_draw_dir = false;

	if (polygon.m_material) {
		m_material = polygon.m_material;
		polygon.m_material->Retain();
	}
}

PolygonShape::PolygonShape(const std::vector<ee::Vector>& vertices)
	: ChainShape(vertices, true)
	, m_material(NULL)
{
	m_draw_dir = false;

	SetMaterialColor(ee::LIGHT_GREEN);
}

PolygonShape::~PolygonShape()
{
	if (m_material) {
		m_material->Release();
	}
	ClearUserData(true);
}

PolygonShape* PolygonShape::Clone() const
{
	return new PolygonShape(*this);
}

bool PolygonShape::IsContain(const ee::Vector& pos) const
{
	return ee::Math2D::IsPointInRect(pos, m_rect) 
		&& ee::Math2D::IsPointInArea(pos, m_vertices);
}

// bool PolygonShape::isIntersect(const ee::Rect& rect) const
// {
// }

void PolygonShape::Translate(const ee::Vector& offset)
{
	ChainShape::Translate(offset);
	if (m_material) {
		m_material->Translate(offset);
	}
}

void PolygonShape::Draw(const ee::Matrix& mt, const ee::RenderColor& color) const
{
	if (m_material) {
		m_material->Draw(mt, color);
		if (ee::SettingData::draw_tris_edge) {
			m_material->DebugDrawTris(mt);
		}
	}

	if (ee::Config::Instance()->GetSettings().visible_tex_edge) {
		ChainShape::Draw(mt, color);
	}
}

ee::PropertySetting* PolygonShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PolygonPropertySetting(stage, this);
}

void PolygonShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	size_t num = value["vertices"]["x"].size();
	m_vertices.resize(num);
	for (size_t i = 0; i < num; ++i) {
		m_vertices[i].x = value["vertices"]["x"][i].asDouble();
		m_vertices[i].y = value["vertices"]["y"][i].asDouble();
	}

	m_loop = true;

	ChainShape::InitBounding();

	LoadMaterial(dir, value["material"]);
}

void PolygonShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		value["vertices"]["x"][i] = m_vertices[i].x;
		value["vertices"]["y"][i] = m_vertices[i].y;
	}

	value["material"] = StoreMaterial(dir);
}

void PolygonShape::ReloadTexture()
{
	m_material->ReloadTexture();
}

void PolygonShape::refresh()
{
	if (m_material) {
		m_material->Refresh(m_vertices);
	}
}

void PolygonShape::SetMaterialColor(const ee::Colorf& color)
{
	if (m_material) {
		m_material->Release();
	}
	m_material = new ColorMaterial(m_vertices, color);
}

void PolygonShape::SetMaterialTexture(ee::ImageSymbol* image)
{
	if (m_material) {
		m_material->Release();
	}
	m_material = new TextureMaterial(m_vertices, image);
}

Json::Value PolygonShape::StoreMaterial(const std::string& dirpath) const
{
	if (m_material) {
		return m_material->Store(dirpath);
	} else {
		return NULL;
	}
}

void PolygonShape::LoadMaterial(const std::string& dirpath, const Json::Value& val)
{
	if (m_material) {
		delete m_material;
		m_material = NULL;
	}

	if (val["type"].isNull()) {
		return;
	}

	if (m_material) {
		m_material->Release();
	}

	std::string type = val["type"].asString();
	if (type == "color") {
		ee::Colorf col;
		col.Unpack(val["color"].asUInt());
		m_material = new ColorMaterial(m_vertices, col);
	} else if (type == "texture") {
		std::string path = val["texture path"].asString();
		ee::ImageSymbol* symbol = static_cast<ee::ImageSymbol*>(
			ee::SymbolMgr::Instance()->FetchSymbol(dirpath + "\\" + path));
		m_material = new TextureMaterial(m_vertices, symbol);
		symbol->Release();
	}
}

}