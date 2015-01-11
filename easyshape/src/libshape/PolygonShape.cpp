#include "PolygonShape.h"
#include "PolygonPropertySetting.h"

namespace libshape
{

PolygonShape::PolygonShape(const PolygonShape& polygon)
	: ChainShape(polygon)
{
	if (polygon.m_material) {
		m_material = polygon.m_material->clone();
	} else {
		m_material = NULL;
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
	delete m_material;

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
	m_material->Translate(offset);
}

void PolygonShape::draw(const d2d::Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	m_material->Draw();

	if (d2d::Settings::bDisplayTrisEdge) {
		m_material->DrawTrisEdge();
	}

	if (d2d::Settings::bDisplayPolyBound) {
		ChainShape::draw(color);
	}
}

d2d::IPropertySetting* PolygonShape::createPropertySetting(d2d::EditPanel* editPanel)
{
	return new PolygonPropertySetting(editPanel, this);
}

void PolygonShape::refresh()
{
	m_material->Refresh(m_vertices);
}

void PolygonShape::SetMaterialColor(const d2d::Colorf& color)
{
	if (m_material) {
		delete m_material;
	}
	m_material = new Color(m_vertices, color);
}

void PolygonShape::SetMaterialTexture(d2d::ImageSymbol* image)
{
	if (m_material) {
		delete m_material;
	}
	m_material = new Texture(m_vertices, image);
}

//////////////////////////////////////////////////////////////////////////
// class PolygonShape::Material
//////////////////////////////////////////////////////////////////////////

PolygonShape::Material::
Material(const Material& material)
	: m_vertices(material.m_vertices)
{
}

void PolygonShape::Material::
Translate(const d2d::Vector& offset)
{
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		m_vertices[i] += offset;
	}
}

void PolygonShape::Material::
Refresh(const std::vector<d2d::Vector>& vertices)
{
	Build(vertices);
}

void PolygonShape::Material::DrawTrisEdge() const
{
	std::vector<d2d::Vector> buf;
	for (size_t i = 0, n = m_vertices.size(); i < n; ++i)
	{
		buf.push_back(m_vertices[i]);
		if (buf.size() == 3)
		{
			d2d::PrimitiveDraw::drawPolyline(buf, d2d::Colorf(0, 1, 0), true);
			buf.clear();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// class PolygonShape::Color
//////////////////////////////////////////////////////////////////////////

PolygonShape::Color::
Color(const std::vector<d2d::Vector>& vertices,
	  const d2d::Colorf& color)
	: m_color(color)
{
	Build(vertices);
}

PolygonShape::Color::
Color(const Color& color)
	: Material(color)
	, m_color(color.m_color)
{
}

PolygonShape::Color* PolygonShape::Color::
clone() const
{
	return new Color(*this);
}

void PolygonShape::Color::
Draw() const
{
	d2d::PrimitiveDraw::drawTriangles(m_vertices, m_color);
}

void PolygonShape::Color::
Build(const std::vector<d2d::Vector>& vertices)
{
	m_vertices.clear();

	std::vector<d2d::Vector> bounding;
	d2d::Math::removeDuplicatePoints(vertices, bounding);

	d2d::Triangulation::normal(bounding, m_vertices);
}

void PolygonShape::Color::
Build(const std::vector<d2d::Vector>& vertices, const std::vector<d2d::Vector>& segments)
{
	m_vertices.clear();

	std::vector<d2d::Vector> bounding;
	d2d::Math::removeDuplicatePoints(vertices, bounding);

	d2d::Triangulation::lines(bounding, segments, m_vertices);
}

//////////////////////////////////////////////////////////////////////////
// class PolygonShape::Texture
//////////////////////////////////////////////////////////////////////////

PolygonShape::Texture::
Texture(const std::vector<d2d::Vector>& vertices,
		d2d::ImageSymbol* image)
{
	image->Retain();
	m_image = image;

	Build(vertices);
}

PolygonShape::Texture::
Texture(const Texture& texture)
	: Material(texture)
	, m_texcoords(texture.m_texcoords)
{
	texture.m_image->Retain();
	m_image = texture.m_image;
}

PolygonShape::Texture::
~Texture()
{
	m_image->Release();
}

PolygonShape::Texture* PolygonShape::Texture::
clone() const
{
	return new Texture(*this);
}

void PolygonShape::Texture::
Translate(const d2d::Vector& offset)
{
	Material::Translate(offset);
	for (int i = 0, n = m_texcoords.size(); i < n; ++i) {
		m_texcoords[i] += offset;
	}
}

void PolygonShape::Texture::
Draw() const
{
	if (m_vertices.empty()) {
		return;
	}
	assert(m_vertices.size() == m_texcoords.size()
		&& m_vertices.size() % 3 == 0);

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->sprite();
	for (int i = 0, n = m_vertices.size(); i < n; i += 3) {
		d2d::Vector vertices[4], texcoords[4];
		for (int j = 0; j < 3; ++j) {
			vertices[j] = m_vertices[i+j];
			texcoords[j] = m_texcoords[i+j];
		}
		vertices[3] = vertices[2];
		texcoords[3] = texcoords[2];
		shader->Draw(vertices, texcoords, m_image->getTextureID());
	}
}

void PolygonShape::Texture::
Build(const std::vector<d2d::Vector>& vertices)
{
	m_vertices.clear();
	m_texcoords.clear();

	std::vector<d2d::Vector> bounding;
	d2d::Math::removeDuplicatePoints(vertices, bounding);

	d2d::Rect r = GetBoundingRegion(bounding);

	std::vector<d2d::Vector> segments;
	GetTexBoundarySegments(r, segments);
	d2d::Triangulation::lines(bounding, segments, m_vertices);

	CalTexcoords(r);
}

void PolygonShape::Texture::
Build(const std::vector<d2d::Vector>& vertices, const std::vector<d2d::Vector>& segments)
{
	m_vertices.clear();
	m_texcoords.clear();

	std::vector<d2d::Vector> bounding;
	d2d::Math::removeDuplicatePoints(vertices, bounding);

	d2d::Rect r = GetBoundingRegion(bounding);

	std::vector<d2d::Vector> _segments;
	GetTexBoundarySegments(r, _segments);
	copy(segments.begin(), segments.end(), back_inserter(_segments));
	d2d::Triangulation::lines(bounding, _segments, m_vertices);

	CalTexcoords(r);
}

d2d::Rect PolygonShape::Texture::
GetBoundingRegion(const std::vector<d2d::Vector>& bounding) const
{
	d2d::Rect r;
	for (int i = 0, n = bounding.size(); i < n; ++i) {
		r.combine(bounding[i]);
	}
	return r;
}

void PolygonShape::Texture::
GetTexBoundarySegments(const d2d::Rect& rect, std::vector<d2d::Vector>& segments)
{
	int width = m_image->getSize().xLength(),
		height = m_image->getSize().yLength();
	for (float x = rect.xMin; x < rect.xMax; x += width)
	{
		segments.push_back(d2d::Vector(x, rect.yMin - 1));
		segments.push_back(d2d::Vector(x, rect.yMax + 1));
	}
	for (float y = rect.yMin; y < rect.yMax; y += height)
	{
		segments.push_back(d2d::Vector(rect.xMin - 1, y));
		segments.push_back(d2d::Vector(rect.xMax + 1, y));
	}
}

void PolygonShape::Texture::
CalTexcoords(const d2d::Rect& rect)
{
	int width = m_image->getSize().xLength(),
		height = m_image->getSize().yLength();
	int index = 0;
	for (size_t i = 0, n = m_vertices.size() / 3; i < n; ++i)
	{
		float cx = 0, cy = 0;
		for (size_t j = 0; j < 3; ++j)
		{
			cx += m_vertices[index + j].x;
			cy += m_vertices[index + j].y;
		}
		cx /= 3;
		cy /= 3;

		int ix = (cx - rect.xMin) / width,
			iy = (cy - rect.yMin) / height;
		d2d::Vector base;
		base.x = rect.xMin + width * ix;
		base.y = rect.yMin + height * iy;

		for (size_t j = 0; j < 3; ++j)
		{
			float tx = (m_vertices[index + j].x - base.x) / width,
				ty = (m_vertices[index + j].y - base.y) / height;
			m_texcoords.push_back(d2d::Vector(tx, ty));
		}

		index +=  3;
	}
}

} // d2d