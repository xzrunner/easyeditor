#include "TextureMaterial.h"

namespace libshape
{

TextureMaterial::TextureMaterial(const std::vector<d2d::Vector>& vertices, d2d::ImageSymbol* image)
{
	image->Retain();
	m_image = image;

	Build(vertices);
}

TextureMaterial::~TextureMaterial()
{
	m_image->Release();
}

Json::Value TextureMaterial::Store(const std::string& dirpath) const
{
	Json::Value val;
	val["type"] = "texture";
	val["texture path"] = d2d::FilenameTools::getRelativePath(dirpath, m_image->getFilepath()).ToStdString();
	return val;
}

void TextureMaterial::Translate(const d2d::Vector& offset)
{
	Material::Translate(offset);
	for (int i = 0, n = m_texcoords.size(); i < n; ++i) {
		m_texcoords[i] += offset;
	}
}

void TextureMaterial::Draw(const d2d::Matrix& mt) const
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
			vertices[j] = d2d::Math::transVector(m_vertices[i+j], mt);
			texcoords[j] = m_texcoords[i+j];
		}
		vertices[3] = vertices[2];
		texcoords[3] = texcoords[2];
		shader->Draw(vertices, texcoords, m_image->getTextureID());
	}
}

void TextureMaterial::Build(const std::vector<d2d::Vector>& vertices)
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

void TextureMaterial::Build(const std::vector<d2d::Vector>& vertices, const std::vector<d2d::Vector>& segments)
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

d2d::Rect TextureMaterial::GetBoundingRegion(const std::vector<d2d::Vector>& bounding) const
{
	d2d::Rect r;
	for (int i = 0, n = bounding.size(); i < n; ++i) {
		r.combine(bounding[i]);
	}
	return r;
}

void TextureMaterial::GetTexBoundarySegments(const d2d::Rect& rect, std::vector<d2d::Vector>& segments)
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

void TextureMaterial::CalTexcoords(const d2d::Rect& rect)
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

}