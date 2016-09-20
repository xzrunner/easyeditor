#include "TexturePolygon.h"
#include "ImageSymbol.h"
#include "RenderParams.h"
#include "Texture.h"

#include <SM_Process.h>
#include <SM_Triangulation.h>
#include <shaderlab.h>

#include <assert.h>

#include <fstream>

namespace s2
{

TexturePolygon::TexturePolygon()
	: m_img(NULL)
{
}

TexturePolygon::TexturePolygon(const TexturePolygon& poly)
	: m_img(NULL)
	, m_texcoords(poly.m_texcoords)
{
	cu::RefCountObjAssign(m_img, const_cast<ImageSymbol*>(poly.m_img));
}

TexturePolygon& TexturePolygon::operator = (const TexturePolygon& poly)
{
	m_img = NULL;
	cu::RefCountObjAssign(m_img, const_cast<ImageSymbol*>(poly.m_img));
	m_texcoords = poly.m_texcoords;
	return *this;
}

TexturePolygon::TexturePolygon(const ImageSymbol* img)
	: m_img(NULL)
{
	cu::RefCountObjAssign(m_img, img);	
}

TexturePolygon::~TexturePolygon()
{
	if (m_img) {
		m_img->RemoveReference();
	}
}

void TexturePolygon::Draw(const sm::mat4& mt, const RenderColor& color) const
{
	if (m_tris.empty()) {
		return;
	}
	assert(m_tris.size() == m_texcoords.size()
		&& m_tris.size() % 3 == 0);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	for (int i = 0, n = m_tris.size(); i < n; i += 3) 
	{
		sm::vec2 vertices[4], texcoords[4];
		for (int j = 0; j < 3; ++j) {
			vertices[j] = mt * m_tris[i+j];
			texcoords[j] = m_texcoords[i+j];
		}
		vertices[3] = vertices[2];
		texcoords[3] = texcoords[2];

		float _texcoords[8];
		int texid;
		m_img->QueryTexcoords(_texcoords, texid);

		float minx = _texcoords[0],
			  miny = _texcoords[1];
		float w = _texcoords[4] - minx,
			  h = _texcoords[5] - miny;
		for (int i = 0; i < 4; ++i) {
			float x = minx + w * texcoords[i].x,
				  y = miny + h * texcoords[i].y;
			texcoords[i].Set(x, y);
		}

		shader->Draw(&vertices[0].x, &texcoords[0].x, texid);
	}

	//sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	//const ee::Camera* cam = ee::CameraMgr::Instance()->GetCamera();
	//if (cam->Type() == "ortho") 
	//{
	//	mgr->SetShader(sl::SPRITE2);
	//	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	//	for (int i = 0, n = m_tris.size(); i < n; i += 3) {
	//		sm::vec2 vertices[4], texcoords[4];
	//		for (int j = 0; j < 3; ++j) {
	//			vertices[j] = mt * m_tris[i+j];
	//			texcoords[j] = m_tris_texcoord[i+j];
	//		}
	//		vertices[3] = vertices[2];
	//		texcoords[3] = texcoords[2];

	//		// 		if (ee::Config::Instance()->IsUseDTex()) {
	//		// 			ee::DynamicTexAndFont::Instance()->Draw(vertices, texcoords, 
	//		// 				m_image->GetFilepath(), m_image->GetTexID());
	//		// 		} else {
	//		shader->Draw(&vertices[0].x, &texcoords[0].x, m_image->GetTexID());
	//		//		}
	//	}
	//}
	//else
	//{
	//	const ee::Pseudo3DCamera* pcam = static_cast<const ee::Pseudo3DCamera*>(cam);
	//	mgr->SetShader(sl::SPRITE3);
	//	sl::Sprite3Shader* shader = static_cast<sl::Sprite3Shader*>(mgr->GetShader(sl::SPRITE3));
	//	for (int i = 0, n = m_tris.size(); i < n; i += 3) {
	//		std::vector<sm::vec3> vertices; 
	//		vertices.resize(3);
	//		std::vector<sm::vec2> texcoords;
	//		texcoords.resize(3);
	//		for (int j = 0; j < 3; ++j) {
	//			sm::vec2 v = mt * m_tris[i+j];
	//			vertices[j] = sm::vec3(v.x, v.y, 0);
	//			texcoords[j] = m_tris_texcoord[i+j];
	//		}
	//		shader->Draw(&vertices[0].x, &texcoords[0].x, m_image->GetTexID());
	//	}
	//}
}

void TexturePolygon::Build()
{
	m_tris.clear();
	m_texcoords.clear();

	std::vector<sm::vec2> outline;
	sm::rm_duplicate_nodes(m_outline, outline);

	sm::rect r;
	for (int i = 0, n = outline.size(); i < n; ++i) {
		r.Combine(outline[i]);
	}

	std::vector<sm::vec2> segments;
	GetTexBoundarySegments(r, segments);
	if (!m_segments.empty()) {
		copy(m_segments.begin(), m_segments.end(), back_inserter(segments));
	}

	sm::triangulate_lines(outline, segments, m_tris);

	CalTexcoords(r);
}

void TexturePolygon::GetTexBoundarySegments(const sm::rect& rect, std::vector<sm::vec2>& segments)
{
	static const int EXTEND = 1;
	sm::vec2 sz = m_img->GetNoTrimedSize();
	for (float x = rect.xmin; x < rect.xmax; x += sz.x)
	{
		segments.push_back(sm::vec2(x, rect.ymin - EXTEND));
		segments.push_back(sm::vec2(x, rect.ymax + EXTEND));
	}
	for (float y = rect.ymin; y < rect.ymax; y += sz.y)
	{
		segments.push_back(sm::vec2(rect.xmin - EXTEND, y));
		segments.push_back(sm::vec2(rect.xmax + EXTEND, y));
	}
}

void TexturePolygon::CalTexcoords(const sm::rect& rect)
{
	sm::vec2 sz = m_img->GetNoTrimedSize();
	int index = 0;
	for (int i = 0, n = m_tris.size() / 3; i < n; ++i)
	{
		float cx = 0, cy = 0;
		for (int j = 0; j < 3; ++j)
		{
			cx += m_tris[index + j].x;
			cy += m_tris[index + j].y;
		}
		cx /= 3;
		cy /= 3;

		int ix = (cx - rect.xmin) / sz.x,
			iy = (cy - rect.ymin) / sz.y;
		sm::vec2 base;
		base.x = rect.xmin + sz.x * ix;
		base.y = rect.ymin + sz.y * iy;

		for (int j = 0; j < 3; ++j)
		{
			float tx = (m_tris[index + j].x - base.x) / sz.x,
				  ty = (m_tris[index + j].y - base.y) / sz.y;
			m_texcoords.push_back(sm::vec2(tx, ty));
		}

		index +=  3;
	}
}

}