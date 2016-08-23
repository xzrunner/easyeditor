#include "PolygonShape.h"
#include "PolygonPropertySetting.h"
#include "ColorMaterial.h"
#include "TextureMaterial.h"
#include "PolylineEditor.h"

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
}

PolygonShape::PolygonShape(const PolygonShape& polygon)
	: eshape::PolylineShape(polygon)
	, s2::PolylineShape(polygon)
{
	if (polygon.m_material) {
		polygon.m_material->AddReference();
		m_material = polygon.m_material;
	}
}

PolygonShape::PolygonShape(const std::vector<sm::vec2>& vertices)
	: s2::PolylineShape(vertices, true)
	, m_material(NULL)
{
	SetMaterialColor(ee::LIGHT_GREEN);
}

PolygonShape& PolygonShape::operator = (const PolygonShape& polygon)
{
	m_vertices = polygon.m_vertices;
	m_closed = true;

	if (polygon.m_material) {
		m_material = polygon.m_material;
		polygon.m_material->AddReference();
	} else {
		m_material = NULL;
	}
	return *this;
}

PolygonShape::~PolygonShape()
{
	if (m_material) {
		m_material->RemoveReference();
	}
	ClearUserData(true);
}

void PolygonShape::Draw(const sm::mat4& mt, const s2::RenderColor& color) const
{
	if (m_material) {
		m_material->Draw(mt, color);
		if (ee::SettingData::draw_tris_edge) {
			m_material->DebugDrawTris(mt);
		}
	}

	if (ee::Config::Instance()->GetSettings().visible_tex_edge) {
		s2::PolylineShape::Draw(mt, color);
	}
}

void PolygonShape::Translate(const sm::vec2& offset)
{
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		m_vertices[i] += offset;
	}
	m_bounding.Translate(offset);

	if (m_material) {
		m_material->Translate(offset);
	}
}

ee::PropertySetting* PolygonShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PolygonPropertySetting(stage, this);
}

void PolygonShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	int num = value["vertices"]["x"].size();
	m_vertices.resize(num);
	for (int i = 0; i < num; ++i) {
		m_vertices[i].x = value["vertices"]["x"][i].asDouble();
		m_vertices[i].y = value["vertices"]["y"][i].asDouble();
	}

	m_closed = true;

	UpdateBounding();

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

void PolygonShape::AddVertex(int index, const sm::vec2& pos)
{
	PolylineEditor::AddVertex(m_vertices, m_bounding, index, pos);
	if (m_material) {
		m_material->Refresh(m_vertices);
	}
}

void PolygonShape::RemoveVertex(const sm::vec2& pos)
{
	PolylineEditor::RemoveVertex(m_vertices, m_bounding, pos);
	if (m_material) {
		m_material->Refresh(m_vertices);
	}
}

void PolygonShape::ChangeVertex(const sm::vec2& from, const sm::vec2& to)
{
	PolylineEditor::ChangeVertex(m_vertices, m_bounding, from, to);
	if (m_material) {
		m_material->Refresh(m_vertices);
	}
}

void PolygonShape::SetVertices(const std::vector<sm::vec2>& vertices)
{
	m_vertices = vertices;
	UpdateBounding();
	if (m_material) {
		m_material->Refresh(m_vertices);
	}
}

void PolygonShape::SetMaterialColor(const s2::Color& color)
{
	if (m_material) {
		m_material->RemoveReference();
	}
	m_material = new ColorMaterial(m_vertices, color);
}

void PolygonShape::SetMaterialTexture(ee::ImageSymbol* image)
{
	if (m_material) {
		m_material->RemoveReference();
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
		m_material->RemoveReference();
	}

	std::string type = val["type"].asString();
	if (type == "color") {
		s2::Color col;
		col.FromRGBA(val["color"].asUInt());
		m_material = new ColorMaterial(m_vertices, col);
	} else if (type == "texture") {
		std::string path = val["texture path"].asString();
		ee::ImageSymbol* sym = static_cast<ee::ImageSymbol*>(
			ee::SymbolMgr::Instance()->FetchSymbol(dirpath + "\\" + path));
		m_material = new TextureMaterial(m_vertices, sym);
		sym->RemoveReference();
	}
}

}