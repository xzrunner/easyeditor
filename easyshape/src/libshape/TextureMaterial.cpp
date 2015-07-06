#include "TextureMaterial.h"

namespace libshape
{

TextureMaterial::TextureMaterial(const std::vector<d2d::Vector>& vertices, d2d::ImageSymbol* image)
{
	image->Retain();
	m_image = image;

	BuildBegin(vertices);
	BuildEnd();
}

TextureMaterial::~TextureMaterial()
{
	m_image->Release();
}

Json::Value TextureMaterial::Store(const std::string& dirpath) const
{
	Json::Value val;
	val["type"] = "texture";
	val["texture path"] = d2d::FilenameTools::getRelativePath(dirpath, m_image->GetFilepath()).ToStdString();
	return val;
}

void TextureMaterial::Translate(const d2d::Vector& offset)
{
	Material::Translate(offset);
	for (int i = 0, n = m_tris_texcoord.size(); i < n; ++i) {
		m_tris_texcoord[i] += offset;
	}
}

void TextureMaterial::Draw(const d2d::Matrix& mt) const
{
	if (m_tris.empty()) {
		return;
	}
	assert(m_tris.size() == m_tris_texcoord.size()
		&& m_tris.size() % 3 == 0);

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->sprite();
	for (int i = 0, n = m_tris.size(); i < n; i += 3) {
		d2d::Vector vertices[4], texcoords[4];
		for (int j = 0; j < 3; ++j) {
			vertices[j] = d2d::Math::transVector(m_tris[i+j], mt);
			texcoords[j] = m_tris_texcoord[i+j];
		}
		vertices[3] = vertices[2];
		texcoords[3] = texcoords[2];

		if (d2d::Config::Instance()->IsUseDTex()) {
			d2d::DynamicTexAndFont::Instance()->Draw(vertices, texcoords, 
				m_image->GetFilepath(), m_image->GetTexID());
		} else {
			shader->Draw(vertices, texcoords, m_image->GetTexID());
		}
	}
}

void TextureMaterial::ReloadTexture()
{
	m_image->ReloadTexture();
}

void TextureMaterial::BuildEnd()
{
	m_tris.clear();
	m_tris_texcoord.clear();

	std::vector<d2d::Vector> outline;
	d2d::Math::removeDuplicatePoints(m_outline, outline);

	d2d::Rect r = GetBoundingRegion(outline);

	std::vector<d2d::Vector> segments;
	GetTexBoundarySegments(r, segments);
	if (!m_segments.empty()) {
		copy(m_segments.begin(), m_segments.end(), back_inserter(segments));
	}

	d2d::Triangulation::lines(outline, segments, m_tris);

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
	static const int EXTEND = 1;
	int width = m_image->GetSize().xLength(),
		height = m_image->GetSize().yLength();
	for (float x = rect.xMin; x < rect.xMax; x += width)
	{
 		segments.push_back(d2d::Vector(x, rect.yMin - EXTEND));
 		segments.push_back(d2d::Vector(x, rect.yMax + EXTEND));
	}
	for (float y = rect.yMin; y < rect.yMax; y += height)
	{
 		segments.push_back(d2d::Vector(rect.xMin - EXTEND, y));
 		segments.push_back(d2d::Vector(rect.xMax + EXTEND, y));
	}
}

void TextureMaterial::CalTexcoords(const d2d::Rect& rect)
{
	int width = m_image->GetSize().xLength(),
		height = m_image->GetSize().yLength();
	int index = 0;
	for (size_t i = 0, n = m_tris.size() / 3; i < n; ++i)
	{
		float cx = 0, cy = 0;
		for (size_t j = 0; j < 3; ++j)
		{
			cx += m_tris[index + j].x;
			cy += m_tris[index + j].y;
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
			float tx = (m_tris[index + j].x - base.x) / width,
				  ty = (m_tris[index + j].y - base.y) / height;
			m_tris_texcoord.push_back(d2d::Vector(tx, ty));
		}

		index +=  3;
	}
}

}