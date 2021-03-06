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

#include <sprite2/RenderParams.h>

namespace eshape
{

PolygonShape::PolygonShape()
{
}

PolygonShape::PolygonShape(const PolygonShape& polygon)
	: s2::Shape(polygon)
	, eshape::EditedPolyShape(polygon)
	, s2::PolygonShape(polygon)
{
}

PolygonShape::PolygonShape(const CU_VEC<sm::vec2>& vertices)
	: s2::PolygonShape(vertices)
{
	SetMaterialColor(ee::LIGHT_GREEN);
}

PolygonShape::~PolygonShape()
{
	ClearUserData(true);
}

void PolygonShape::Draw(cooking::DisplayList* dlist, const s2::RenderParams& rp) const
{
	if (m_poly) {
		m_poly->Draw(dlist, rp);
		if (ee::SettingData::draw_tris_edge) {
			m_poly->DebugDraw(rp.mt);
		}
	}

	if (ee::Config::Instance()->GetSettings().visible_tex_edge) {
		s2::PolylineShape::Draw(dlist, rp);
	}
}

void PolygonShape::Translate(const sm::vec2& offset)
{
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		m_vertices[i] += offset;
	}
	m_bounding.Translate(offset);

	if (m_poly) {
		dynamic_cast<Material*>(m_poly.get())->Translate(offset);
	}
}

ee::PropertySetting* PolygonShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PolygonPropertySetting(stage, 
		std::dynamic_pointer_cast<eshape::PolygonShape>(shared_from_this()));
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
}

void PolygonShape::AddVertex(int index, const sm::vec2& pos)
{
	PolylineEditor::AddVertex(m_vertices, m_bounding, index, pos);
	if (m_poly) {
		Material* mat = dynamic_cast<Material*>(m_poly.get());
		mat->Clear();
		mat->SetOutline(m_vertices);
		mat->Build();
	}
}

void PolygonShape::RemoveVertex(const sm::vec2& pos)
{
	PolylineEditor::RemoveVertex(m_vertices, m_bounding, pos);
	if (m_poly) {
		Material* mat = dynamic_cast<Material*>(m_poly.get());
		mat->Clear();
		mat->SetOutline(m_vertices);
		mat->Build();
	}
}

void PolygonShape::ChangeVertex(const sm::vec2& from, const sm::vec2& to)
{
	PolylineEditor::ChangeVertex(m_vertices, m_bounding, from, to);
	if (m_poly) {
		Material* mat = dynamic_cast<Material*>(m_poly.get());
		mat->Clear();
		mat->SetOutline(m_vertices);
		mat->Build();
	}
}

void PolygonShape::SetVertices(const CU_VEC<sm::vec2>& vertices)
{
	m_vertices = vertices;
	UpdateBounding();
	if (m_poly) {
		Material* mat = dynamic_cast<Material*>(m_poly.get());
		mat->Clear();
		mat->SetOutline(m_vertices);
		mat->Build();
	}
}

void PolygonShape::SetMaterialColor(const pt2::Color& color)
{
	m_poly = std::make_unique<ColorMaterial>(color);
	m_poly->SetOutline(m_vertices);
	m_poly->Build();
}

void PolygonShape::SetMaterialTexture(const std::shared_ptr<ee::ImageSymbol>& image)
{
	m_poly = std::make_unique<TextureMaterial>(image);
	m_poly->SetOutline(m_vertices);
	m_poly->Build();
}

Json::Value PolygonShape::StoreMaterial(const std::string& dirpath) const
{
	if (m_poly) {
		return dynamic_cast<Material*>(m_poly.get())->Store(dirpath);
	} else {
		return NULL;
	}
}

void PolygonShape::LoadMaterial(const std::string& dirpath, const Json::Value& val)
{
	m_poly.reset();

	if (val["type"].isNull()) {
		return;
	}

	std::string type = val["type"].asString();
	if (type == "color") {
		pt2::Color col;
		col.FromRGBA(val["color"].asUInt());
		m_poly = std::make_unique<ColorMaterial>(col);
		m_poly->SetOutline(m_vertices);
		m_poly->Build();
	} else if (type == "texture") {
		std::string path = val["texture path"].asString();
		auto sym = std::dynamic_pointer_cast<ee::ImageSymbol>(
			ee::SymbolMgr::Instance()->FetchSymbol(dirpath + "\\" + path));
		m_poly = std::make_unique<TextureMaterial>(sym);
		m_poly->SetOutline(m_vertices);
		m_poly->Build();
	}
}

const Material* PolygonShape::GetMaterial() const 
{ 
	return dynamic_cast<const Material*>(m_poly.get()); 
}

}