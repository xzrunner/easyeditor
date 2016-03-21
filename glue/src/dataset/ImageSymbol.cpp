#include "ImageSymbol.h"
#include "Texture.h"

#include "math/Math.h"
#include "render/ShaderMgr.h"
#include "render/RenderContext.h"
#include "render/Camera.h"
#include "render/SpriteShader.h"
#include "render/Sprite3Shader.h"

// debug
#include "math/Matrix.h"

namespace glue
{

ImageSymbol::ImageSymbol(const std::string& filepath)
	: m_filepath(filepath)
	, m_tex(NULL)
{
	Load();
}

void ImageSymbol::Draw(const mat4& mt) const
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
		positions[i] = Math::TransVector(positions[i], mt);
	}

	vec2 texcoords[4];
	texcoords[0] = vec2(0, 0);
	texcoords[1] = vec2(1, 0);
	texcoords[2] = vec2(1, 1);
	texcoords[3] = vec2(0, 1);

	ShaderMgr* smgr = ShaderMgr::Instance();
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
		smgr->SetShader(ShaderMgr::SPRITE);
		SpriteShader* shader = static_cast<SpriteShader*>(smgr->GetShader(ShaderMgr::SPRITE));
		shader->Draw(positions, texcoords, texid);
//	}
}

void ImageSymbol::ModelDraw(const mat4& mt) const
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
		positions[i] = Math::TransVector(positions[i], mt);
	}

	float z[4];
	z[0] = -4;
	z[1] = -2;
	z[2] = -2;
	z[3] = -4;

	std::vector<vec3> positions3;
	positions3.push_back(vec3(positions[0].x, positions[0].y, z[0]));
	positions3.push_back(vec3(positions[1].x, positions[1].y, z[1]));
	positions3.push_back(vec3(positions[2].x, positions[2].y, z[2]));
	positions3.push_back(vec3(positions[0].x, positions[0].y, z[0]));
	positions3.push_back(vec3(positions[2].x, positions[2].y, z[2]));
	positions3.push_back(vec3(positions[3].x, positions[3].y, z[3]));

	//////////////////////////////////////////////////////////////////////////

// 	int w = 800, h = 480;
// 
// 	float _hw = w * 0.5f;
// 	float _hh = h * 0.5f;
// 
// 	mat4 mat = Matrix4<float>::Perspective(-_hw, _hw, -_hh, _hh, Sprite3Shader::NEAR, Sprite3Shader::FAR);
// 
// 	std::vector<vec3> test;
// 	for (int i = 0, n = positions3.size(); i < n; ++i) {
// 		test.push_back(mat * positions3[i]);
// 	}

	//////////////////////////////////////////////////////////////////////////

	std::vector<vec2> texcoords;
	texcoords.push_back(vec2(0, 0));
	texcoords.push_back(vec2(1, 0));
	texcoords.push_back(vec2(1, 1));
	texcoords.push_back(vec2(0, 0));
	texcoords.push_back(vec2(1, 1));
	texcoords.push_back(vec2(0, 1));

	ShaderMgr* smgr = ShaderMgr::Instance();
	smgr->SetShader(ShaderMgr::SPRITE3);
	Sprite3Shader* shader = static_cast<Sprite3Shader*>(smgr->GetShader(ShaderMgr::SPRITE3));
	shader->Draw(positions3, texcoords, texid);
}

void ImageSymbol::Load()
{
	m_tex = new glue::Texture();
	m_tex->Load(m_filepath);
}

}