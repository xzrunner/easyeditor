#include "ImageSymbol.h"
#include "Texture.h"

#include "render/RenderContext.h"
#include "render/SpriteShader.h"
#include "render/Sprite3Shader.h"

#include <shaderlab.h>
#include <sm.h>

namespace glue
{

ImageSymbol::ImageSymbol(const std::string& filepath)
	: m_filepath(filepath)
	, m_tex(NULL)
{
	Load();
}

void ImageSymbol::Draw(const sm_mat4& mt) const
{
	RID texid = m_tex->GetID();

	vec2 positions[4];
	float hw = m_tex->GetWidth() * 0.5f,
		  hh = m_tex->GetHeight() * 0.5f;
	positions[0] = vec2(-hw, -hh);
	positions[1] = vec2( hw, -hh);
	positions[2] = vec2( hw,  hh);
	positions[3] = vec2(-hw,  hh);
	for (int i = 0; i < 4; ++i) {
		sm_vec2 v;
		memcpy(&v, &positions[i], sizeof(v));
		sm_vec2_mul(&v, &mt);
		memcpy(&positions[i], &v, sizeof(v));
	}

	vec2 texcoords[4];
	texcoords[0] = vec2(0, 0);
	texcoords[1] = vec2(1, 0);
	texcoords[2] = vec2(1, 1);
	texcoords[3] = vec2(0, 1);

// 	if (smgr->IsBlendShader())
// 	{
// 		vec2 vertices_scr[4];
// 		float img_hw = m_tex->GetWidth() * 0.5f,
// 			  img_hh = m_tex->GetHeight() * 0.5f;
// 		vertices_scr[0] = Math::TransVector(vec2(-img_hw, -img_hh), mt);
// 		vertices_scr[1] = Math::TransVector(vec2( img_hw, -img_hh), mt);
// 		vertices_scr[2] = Math::TransVector(vec2( img_hw,  img_hh), mt);
// 		vertices_scr[3] = Math::TransVector(vec2(-img_hw,  img_hh), mt);
// 
// 		RenderContext* ctx = RenderContext::Instance();
// 		vec2 tex_coords_base[4];
// 		int w = ctx->GetWidth(),
// 			h = ctx->GetHeight();
//  		for (int i = 0; i < 4; ++i) {
//  			tex_coords_base[i] = ctx->GetCamera()->TransPosProjectToScreen(vertices_scr[i], w, h);
//  			tex_coords_base[i].y = h - 1 - tex_coords_base[i].y;
//  			tex_coords_base[i].x /= w;
//  			tex_coords_base[i].y /= h;
//  			tex_coords_base[i].x = std::min(std::max(0.0f, tex_coords_base[i].x), 1.0f);
//  			tex_coords_base[i].y = std::min(std::max(0.0f, tex_coords_base[i].y), 1.0f);
//  		}
// 		smgr->BlendDraw(vertices_scr, texcoords, tex_coords_base, texid, );
// 	}
// 	else
// 	{
		sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
		sl_mgr->SetShader(sl::SPRITE2);
		sl::Sprite2Shader* sl_shader = static_cast<sl::Sprite2Shader*>(sl_mgr->GetShader(sl::SPRITE2));
		sl_shader->Draw(&positions[0].x, &texcoords[0].x, texid);
//	}
}

void ImageSymbol::Draw25(const sm_mat4& mt) const
{
	RID texid = m_tex->GetID(); 

	vec2 positions[4];
	float hw = m_tex->GetWidth() * 0.5f,
		hh = m_tex->GetHeight() * 0.5f;
	positions[0] = vec2(-hw, -hh);
	positions[1] = vec2( hw, -hh);
	positions[2] = vec2( hw,  hh);
	positions[3] = vec2(-hw,  hh);
	for (int i = 0; i < 4; ++i) {
		sm_vec2 v;
		memcpy(&v, &positions[i], sizeof(v));
		sm_vec2_mul(&v, &mt);
	}

	float z[4];
	z[0] = 250;
	z[1] = 0;
	z[2] = 0;
	z[3] = 250;

	std::vector<vec3> positions3;
	positions3.push_back(vec3(positions[0].x, positions[0].y, z[0]));
	positions3.push_back(vec3(positions[1].x, positions[1].y, z[1]));
	positions3.push_back(vec3(positions[2].x, positions[2].y, z[2]));
	positions3.push_back(vec3(positions[0].x, positions[0].y, z[0]));
	positions3.push_back(vec3(positions[2].x, positions[2].y, z[2]));
	positions3.push_back(vec3(positions[3].x, positions[3].y, z[3]));

	std::vector<vec2> texcoords;
	texcoords.push_back(vec2(0, 0));
	texcoords.push_back(vec2(1, 0));
	texcoords.push_back(vec2(1, 1));
	texcoords.push_back(vec2(0, 0));
	texcoords.push_back(vec2(1, 1));
	texcoords.push_back(vec2(0, 1));

	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->SetShader(sl::SPRITE3);
	sl::Sprite3Shader* sl_shader = static_cast<sl::Sprite3Shader*>(sl_mgr->GetShader());
	sl_shader->Draw(&positions3[0].x, &texcoords[0].x, texid);
}

void ImageSymbol::Load()
{
	m_tex = new Texture();
	m_tex->Load(m_filepath);
}

}