#include "TextureMaterial.h"

#include <ee/ImageSymbol.h>
#include <ee/FileHelper.h>
#include <ee/Math2D.h>
#include <ee/Triangulation.h>
#include <ee/Camera.h>
#include <ee/CameraMgr.h>
#include <ee/Pseudo3DCamera.h>

#include <shaderlab.h>

namespace eshape
{

TextureMaterial::TextureMaterial(const std::vector<ee::Vector>& vertices, ee::ImageSymbol* image)
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
	val["texture path"] = ee::FileHelper::GetRelativePath(dirpath, m_image->GetFilepath());
	return val;
}

void TextureMaterial::Translate(const ee::Vector& offset)
{
	Material::Translate(offset);
	for (int i = 0, n = m_tris_texcoord.size(); i < n; ++i) {
		m_tris_texcoord[i] += offset;
	}
}

void TextureMaterial::Draw(const ee::Matrix& mt, const ee::RenderColor& color) const
{
	if (m_tris.empty()) {
		return;
	}
	assert(m_tris.size() == m_tris_texcoord.size()
		&& m_tris.size() % 3 == 0);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	const ee::Camera* cam = ee::CameraMgr::Instance()->GetCamera();
	if (cam->Type() == "ortho") 
	{
		mgr->SetShader(sl::SPRITE2);
		sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
		for (int i = 0, n = m_tris.size(); i < n; i += 3) {
			ee::Vector vertices[4], texcoords[4];
			for (int j = 0; j < 3; ++j) {
				vertices[j] = ee::Math2D::TransVector(m_tris[i+j], mt);
				texcoords[j] = m_tris_texcoord[i+j];
			}
			vertices[3] = vertices[2];
			texcoords[3] = texcoords[2];

			// 		if (ee::Config::Instance()->IsUseDTex()) {
			// 			ee::DynamicTexAndFont::Instance()->Draw(vertices, texcoords, 
			// 				m_image->GetFilepath(), m_image->GetTexID());
			// 		} else {
			shader->Draw(&vertices[0].x, &texcoords[0].x, m_image->GetTexID());
			//		}
		}
	}
	else
	{
		const ee::Pseudo3DCamera* pcam = static_cast<const ee::Pseudo3DCamera*>(cam);
		mgr->SetShader(sl::SPRITE3);
		sl::Sprite3Shader* shader = static_cast<sl::Sprite3Shader*>(mgr->GetShader(sl::SPRITE3));
		for (int i = 0, n = m_tris.size(); i < n; i += 3) {
			std::vector<sm::vec3> vertices; 
			vertices.resize(3);
			std::vector<ee::Vector> texcoords;
			texcoords.resize(3);
			for (int j = 0; j < 3; ++j) {
				ee::Vector v = ee::Math2D::TransVector(m_tris[i+j], mt);
				vertices[j] = sm::vec3(v.x, v.y, 0);
				texcoords[j] = m_tris_texcoord[i+j];
			}
			shader->Draw(&vertices[0].x, &texcoords[0].x, m_image->GetTexID());
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

	std::vector<ee::Vector> outline;
	ee::Math2D::RemoveDuplicatePoints(m_outline, outline);

	ee::Rect r = GetBoundingRegion(outline);

	std::vector<ee::Vector> segments;
	GetTexBoundarySegments(r, segments);
	if (!m_segments.empty()) {
		copy(m_segments.begin(), m_segments.end(), back_inserter(segments));
	}

	ee::Triangulation::Lines(outline, segments, m_tris);

	CalTexcoords(r);
}

ee::Rect TextureMaterial::GetBoundingRegion(const std::vector<ee::Vector>& bounding) const
{
	ee::Rect r;
	for (int i = 0, n = bounding.size(); i < n; ++i) {
		r.Combine(bounding[i]);
	}
	return r;
}

void TextureMaterial::GetTexBoundarySegments(const ee::Rect& rect, std::vector<ee::Vector>& segments)
{
	static const int EXTEND = 1;
	int width = m_image->GetSize().Width(),
		height = m_image->GetSize().Height();
	for (float x = rect.xmin; x < rect.xmax; x += width)
	{
 		segments.push_back(ee::Vector(x, rect.ymin - EXTEND));
 		segments.push_back(ee::Vector(x, rect.ymax + EXTEND));
	}
	for (float y = rect.ymin; y < rect.ymax; y += height)
	{
 		segments.push_back(ee::Vector(rect.xmin - EXTEND, y));
 		segments.push_back(ee::Vector(rect.xmax + EXTEND, y));
	}
}

void TextureMaterial::CalTexcoords(const ee::Rect& rect)
{
	int width = m_image->GetSize().Width(),
		height = m_image->GetSize().Height();
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

		int ix = (cx - rect.xmin) / width,
			iy = (cy - rect.ymin) / height;
		ee::Vector base;
		base.x = rect.xmin + width * ix;
		base.y = rect.ymin + height * iy;

		for (size_t j = 0; j < 3; ++j)
		{
			float tx = (m_tris[index + j].x - base.x) / width,
				  ty = (m_tris[index + j].y - base.y) / height;
			m_tris_texcoord.push_back(ee::Vector(tx, ty));
		}

		index +=  3;
	}
}

}