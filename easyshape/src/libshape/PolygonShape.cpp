#include "PolygonShape.h"
#include "PolygonPropertySetting.h"
#include "ColorMaterial.h"
#include "TextureMaterial.h"

namespace libshape
{

PolygonShape::PolygonShape(const PolygonShape& polygon)
	: ChainShape(polygon)
	, m_material(NULL)
{
	if (polygon.m_material) {
		m_material = polygon.m_material;
		polygon.m_material->Retain();
	}
}

PolygonShape::PolygonShape(const std::vector<d2d::Vector>& vertices)
	: ChainShape(vertices, true)
	, m_material(NULL)
{
	SetMaterialColor(d2d::LIGHT_GREY);
}

PolygonShape::~PolygonShape()
{
	if (m_material) {
		m_material->Release();
	}
	clearUserData(true);
}

PolygonShape* PolygonShape::clone() const
{
	return new PolygonShape(*this);
}

bool PolygonShape::isContain(const d2d::Vector& pos) const
{
	return d2d::Math::isPointInArea(pos, m_vertices);
}

// bool PolygonShape::isIntersect(const d2d::Rect& rect) const
// {
// }

void PolygonShape::Translate(const d2d::Vector& offset)
{
	ChainShape::Translate(offset);
	if (m_material) {
		m_material->Translate(offset);
	}
}

void PolygonShape::draw(const d2d::Matrix& mt,
						const d2d::Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	if (m_material) {
		m_material->Draw(mt);
	}

	if (d2d::Settings::bDisplayTrisEdge && m_material) {
		m_material->DebugDrawTris(mt);
	}

	if (d2d::Settings::bDisplayPolyBound) {
		ChainShape::draw(mt, color);
	}
}

d2d::IPropertySetting* PolygonShape::createPropertySetting(d2d::EditPanel* editPanel)
{
	return new PolygonPropertySetting(editPanel, this);
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

void PolygonShape::SetMaterialColor(const d2d::Colorf& color)
{
	if (m_material) {
		m_material->Release();
	}
	m_material = new ColorMaterial(m_vertices, color);
}

void PolygonShape::SetMaterialTexture(d2d::ImageSymbol* image)
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
		d2d::Colorf col;
		col.unpack(val["color"].asUInt());
		m_material = new ColorMaterial(m_vertices, col);
	} else if (type == "texture") {
		std::string path = val["texture path"].asString();
		d2d::ImageSymbol* symbol = static_cast<d2d::ImageSymbol*>(
			d2d::SymbolMgr::Instance()->fetchSymbol(dirpath + "/" + path));
		m_material = new TextureMaterial(m_vertices, symbol);
		symbol->Release();
	}
}

} // d2d