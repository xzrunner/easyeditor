#include "DrawGaussianBlur.h"
#include "S2_Sprite.h"
#include "RenderCtxStack.h"
#include "BoundingBox.h"
#include "RenderParams.h"
#include "FilterFactory.h"
#include "DrawNode.h"

#include <shaderlab.h>
#include <dtex_facade.h>

namespace s2
{

void DrawGaussianBlur::Draw(const Sprite* spr, const RenderParams& params, int iterations)
{
	DrawToFbo0(spr, params, iterations);
	DrawToScreen(true, spr->GetPosition());
}

void DrawGaussianBlur::DrawToFbo0(const Sprite* spr, const RenderParams& params, int iterations)
{
	sl::ShaderMgr::Instance()->GetShader()->Commit();

	DrawInit(spr, params, true);

	int edge = dtexf_t0_get_texture_size();
	RenderCtxStack::Instance()->Push(RenderCtx(edge, edge));

	DrawInit(spr, params, true);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::FILTER);

	sm::vec2 sz = spr->GetBounding()->GetSize().Size();
	for (int i = 0; i < iterations; ++i) {
		DrawBetweenFBO(true, true, params.color, sz.x);
		DrawBetweenFBO(false, false, params.color, sz.y);
	}

	RenderCtxStack::Instance()->Pop();
}

void DrawGaussianBlur::DrawToScreen(bool is_target0, const sm::vec2& offset)
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

void DrawGaussianBlur::DrawInit(const Sprite* spr, const RenderParams& params, bool is_target0)
{
	if (is_target0) {
		dtexf_t0_bind();
		dtexf_t0_clear(0, -2, 2, 0);
	} else {
		dtexf_t1_bind();
		dtexf_t1_clear(0, -2, 2, 0);
	}

	RenderParams _params = params;
	const sm::vec2& offset = spr->GetPosition();
	_params.mt.Translate(-offset.x, -offset.y, 0);
	_params.set_shader = false;
	_params.shader.filter = FilterFactory::Instance()->GetTemp(FM_NULL);

	RenderFilter* ori_filter = spr->Shader().filter;
	const_cast<Sprite*>(spr)->Shader().filter = FilterFactory::Instance()->GetTemp(FM_NULL);
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	DrawNode::Draw(spr, _params);
	const_cast<Sprite*>(spr)->Shader().filter = ori_filter;

	if (is_target0) {
		dtexf_t0_unbind();
	} else {
		dtexf_t1_unbind();
	}
}

void DrawGaussianBlur::DrawBetweenFBO(bool is_t0_to_t1, bool hori, const RenderColor& col, float tex_size)
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