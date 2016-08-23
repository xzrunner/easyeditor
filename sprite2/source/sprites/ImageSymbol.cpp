#include "ImageSymbol.h"
#include "Texture.h"
#include "RenderParams.h"
#include "S2_Sprite.h"

#include <SM_Matrix.h>
#include <shaderlab.h>

#include <assert.h>

namespace s2
{

ImageSymbol::ImageSymbol()
{
}

void ImageSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(m_size.xmin, m_size.ymin);
	vertices[1] = sm::vec2(m_size.xmin, m_size.ymax);
	vertices[2] = sm::vec2(m_size.xmax, m_size.ymax);
	vertices[3] = sm::vec2(m_size.xmax, m_size.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = params.mt * vertices[i];
	}

	float texcoords[8];
	int texid;
	QueryTexcoords(texcoords, texid);
	
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (mgr->GetShaderType() == sl::BLEND) {
		DrawBlend(params, vertices, texcoords, texid);
	} else {
		if (IsOrthoCam()) {
			DrawOrtho(params, vertices, texcoords, texid);
		} else {
			DrawPseudo3D(params, vertices, texcoords, texid);
		}
	}
}

sm::rect ImageSymbol::GetBounding(const Sprite* spr) const
{
	return m_size;
}

void ImageSymbol::InitTex(Texture* tex, const Quad& quad, const sm::vec2& offset)
{
	m_tex = tex;
	m_quad = quad;
	m_offset = offset;

	sm::vec2 sz = m_tex->GetSize();
	m_size.xmin = m_quad.xmin - sz.x * 0.5f + m_offset.x;
	m_size.ymin = m_quad.ymin - sz.y * 0.5f + m_offset.y;
	m_size.xmax = m_quad.xmax - sz.x * 0.5f + m_offset.x;
	m_size.ymax = m_quad.ymax - sz.y * 0.5f + m_offset.y;
}

void ImageSymbol::DrawBlend(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::BlendShader* shader = static_cast<sl::BlendShader*>(mgr->GetShader(sl::BLEND));
	shader->SetColor(params.color.mul.ToABGR(), params.color.add.ToABGR());

	for (int i = 0; i < 4; ++i) {
		vertices[i] += params.vertex_offset;
	}

	sm::vec2 vertices_scr[4];
	sm::vec2 sz = m_tex->GetSize();
	float img_hw = sz.x * 0.5f,
		  img_hh = sz.y * 0.5f;
	vertices_scr[0] = params.mt * sm::vec2(-img_hw, -img_hh);
	vertices_scr[1] = params.mt * sm::vec2(-img_hw,  img_hh);
	vertices_scr[2] = params.mt * sm::vec2( img_hw,  img_hh);
	vertices_scr[3] = params.mt * sm::vec2( img_hw, -img_hh);

	sm::vec2 tex_coords_base[4];
	int w, h;
	GetScreenSize(w, h);
	for (int i = 0; i < 4; ++i) {
		Proj2Screen(vertices_scr[i].x, vertices_scr[i].y, w, h, tex_coords_base[i].x, tex_coords_base[i].y);
		tex_coords_base[i].y = h - 1 - tex_coords_base[i].y;
		tex_coords_base[i].x /= w;
		tex_coords_base[i].y /= h;
	}
	shader->Draw(&vertices[0].x, texcoords, &tex_coords_base[0].x, texid, GetScreenCacheTexid());
}

void ImageSymbol::DrawOrtho(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
// 	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
// 	shader->SetColor(trans.color);
	if (mgr->GetShaderType() == sl::FILTER) {
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
		shader->SetColor(params.color.mul.ToABGR(), params.color.add.ToABGR());
		shader->Draw(&vertices[0].x, texcoords, texid);
	} else if (mgr->GetShaderType() == sl::SPRITE2) {
		sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
		shader->SetColor(params.color.mul.ToABGR(), params.color.add.ToABGR());
		shader->SetColorMap(params.color.rmap.ToABGR(),params.color.gmap.ToABGR(), params.color.bmap.ToABGR());
		shader->Draw(&vertices[0].x, texcoords, texid);
	}
}

void ImageSymbol::DrawPseudo3D(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();

	float angle = GetP3dCamAngle();
	float z[4];
	params.camera.CalculateZ(angle, vertices, z);

	std::vector<sm::vec3> _vertices;
	_vertices.push_back(sm::vec3(vertices[0].x, vertices[0].y, z[0]));
	_vertices.push_back(sm::vec3(vertices[1].x, vertices[1].y, z[1]));
	_vertices.push_back(sm::vec3(vertices[2].x, vertices[2].y, z[2]));
	_vertices.push_back(sm::vec3(vertices[0].x, vertices[0].y, z[0]));
	_vertices.push_back(sm::vec3(vertices[2].x, vertices[2].y, z[2]));
	_vertices.push_back(sm::vec3(vertices[3].x, vertices[3].y, z[3]));

	std::vector<sm::vec2> _texcoords;
	_texcoords.push_back(sm::vec2(texcoords[0], texcoords[1]));
	_texcoords.push_back(sm::vec2(texcoords[2], texcoords[3]));
	_texcoords.push_back(sm::vec2(texcoords[4], texcoords[5]));
	_texcoords.push_back(sm::vec2(texcoords[0], texcoords[1]));
	_texcoords.push_back(sm::vec2(texcoords[4], texcoords[5]));
	_texcoords.push_back(sm::vec2(texcoords[6], texcoords[7]));

	mgr->SetShader(sl::SPRITE3);
	sl::Sprite3Shader* shader = static_cast<sl::Sprite3Shader*>(mgr->GetShader(sl::SPRITE3));
	shader->SetColor(params.color.mul.ToABGR(), params.color.add.ToABGR());
	shader->SetColorMap(params.color.rmap.ToABGR(), params.color.gmap.ToABGR(), params.color.bmap.ToABGR());
	shader->Draw(&_vertices[0].x, &_texcoords[0].x, texid);
}

}