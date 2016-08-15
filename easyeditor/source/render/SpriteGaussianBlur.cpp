#include "SpriteGaussianBlur.h"
#include "EE_DTex.h"
#include "RenderContextStack.h"
#include "GL.h"
#include "Sprite.h"
#include "SpriteRenderer.h"

#include <shaderlab.h>
#include <sprite2/RenderParams.h>
#include <sprite2/FilterFactory.h>
#include <dtex_facade.h>

#include <assert.h>

namespace ee
{

void SpriteGaussianBlur::Draw(const Sprite* spr, const s2::RenderParams& params, int iterations)
{
	DrawToFbo0(spr, params, iterations);
	DrawToScreen(true, spr->GetPosition());
}

void SpriteGaussianBlur::DrawToFbo0(const Sprite* spr, const s2::RenderParams& params, int iterations)
{
	sl::ShaderMgr::Instance()->GetShader()->Commit();

	DrawInit(spr, params, true);

	RenderContextStack* rc = RenderContextStack::Instance();

	sm::vec2 ori_offset;
	float ori_scale;
	rc->GetModelView(ori_offset, ori_scale);

	int ori_width, ori_height;
	rc->GetProjection(ori_width, ori_height);

	rc->SetModelView(sm::vec2(0, 0), 1);
	int edge = dtexf_t0_get_texture_size();
	rc->SetProjection(edge, edge);
	GL::Viewport(0, 0, edge, edge);

	DrawInit(spr, params, true);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::FILTER);

	float tex_width = spr->GetBounding()->Width(),
		  tex_height = spr->GetBounding()->Height();
	for (int i = 0; i < iterations; ++i) {
		DrawBetweenFBO(true, true, params.color, tex_width);
		DrawBetweenFBO(false, false, params.color, tex_height);
	}

	rc->SetModelView(ori_offset, ori_scale);
	rc->SetProjection(ori_width, ori_height);
	GL::Viewport(0, 0, ori_width, ori_height);
}

void SpriteGaussianBlur::DrawToScreen(bool is_target0, const sm::vec2& offset)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());

	sm::vec2 vertices[4];
	vertices[0].Set(-512, -512);
	vertices[1].Set( 512, -512);
	vertices[2].Set( 512,  512);
	vertices[3].Set(-512,  512);
	for (int i = 0; i < 4; ++i) {
		vertices[i] += offset;
	}

	sm::vec2 texcoords[4];
	texcoords[0].Set(0, 0);
	texcoords[1].Set(1, 0);
	texcoords[2].Set(1, 1);
	texcoords[3].Set(0, 1);

	int tex_id;
	if (is_target0) {
		tex_id = dtexf_t0_get_texture_id();
	} else {
		tex_id = dtexf_t1_get_texture_id();
	}
	shader->Draw(&vertices[0].x, &texcoords[0].x, tex_id);

	shader->Commit();
}

void SpriteGaussianBlur::DrawInit(const Sprite* spr, const s2::RenderParams& params, bool is_target0)
{
	if (is_target0) {
		dtexf_t0_bind();
		dtexf_t0_clear(0, -2, 2, 0);
	} else {
		dtexf_t1_bind();
		dtexf_t1_clear(0, -2, 2, 0);
	}

	s2::RenderParams _params = params;
	const sm::vec2& offset = spr->GetPosition();
	_params.mt.Translate(-offset.x, -offset.y, 0);
	_params.set_shader = false;
	_params.shader.filter = s2::FilterFactory::Instance()->GetTemp(s2::FM_NULL);

	s2::RenderFilter* ori_filter = spr->GetShader().filter;
	const_cast<Sprite*>(spr)->GetShader().filter = s2::FilterFactory::Instance()->GetTemp(s2::FM_NULL);
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	SpriteRenderer::Draw(spr, _params);
	const_cast<Sprite*>(spr)->GetShader().filter = ori_filter;

	if (is_target0) {
		dtexf_t0_unbind();
	} else {
		dtexf_t1_unbind();
	}
}

void SpriteGaussianBlur::DrawBetweenFBO(bool is_t0_to_t1, bool hori, const s2::RenderColor& col, float tex_size)
{
	int from_tex_sz;
	int from_tex_id;
	if (is_t0_to_t1) {
		from_tex_sz = dtexf_t0_get_texture_size();
		from_tex_id = dtexf_t0_get_texture_id();
		dtexf_t1_bind();
		dtexf_t1_clear(0, -2, 2, 0);
	} else {
		from_tex_sz = dtexf_t1_get_texture_size();
		from_tex_id = dtexf_t1_get_texture_id();
		dtexf_t0_bind();
		dtexf_t0_clear(0, -2, 2, 0);
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
	if (hori) {
		shader->SetMode(sl::FM_GAUSSIAN_BLUR_HORI);
		sl::GaussianBlurHoriProg* prog = static_cast<sl::GaussianBlurHoriProg*>(shader->GetProgram(sl::FM_GAUSSIAN_BLUR_HORI));
		prog->SetTexWidth(from_tex_sz);
//		prog->SetTexWidth(tex_size);
	} else {
		shader->SetMode(sl::FM_GAUSSIAN_BLUR_VERT);
		sl::GaussianBlurVertProg* prog = static_cast<sl::GaussianBlurVertProg*>(shader->GetProgram(sl::FM_GAUSSIAN_BLUR_VERT));
		prog->SetTexHeight(from_tex_sz);
//		prog->SetTexHeight(tex_size);		
	}
	shader->SetColor(col.mul.ToABGR(), col.add.ToABGR());

	sm::vec2 vertices[4], texcoords[4];
	vertices[0].Set(-512, -512);
	vertices[1].Set( 512, -512);
	vertices[2].Set( 512,  512);
	vertices[3].Set(-512,  512);
	texcoords[0].Set(0, 0);
	texcoords[1].Set(1, 0);
	texcoords[2].Set(1, 1);
	texcoords[3].Set(0, 1);

	shader->Draw(&vertices[0].x, &texcoords[0].x, from_tex_id);

	shader->Commit();

	if (is_t0_to_t1) {
		dtexf_t1_unbind();
	} else {
		dtexf_t0_unbind();
	}
}

}