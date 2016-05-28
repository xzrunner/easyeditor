#include "SpriteGaussianBlur.h"
#include "EE_DTex.h"
#include "RenderContextStack.h"
#include "DTexC1.h"
#include "GL.h"
#include "Sprite.h"
#include "SpriteRenderer.h"

#include <shaderlab.h>
#include <sprite2/RenderParams.h>

namespace ee
{

void SpriteGaussianBlur::Draw(const Sprite* spr, const s2::RenderParams& params)
{
	DrawToFbo0(spr, params);
	DrawToScreen(DTex::Instance()->GetFbo0(), spr->GetPosition());
}

void SpriteGaussianBlur::DrawToFbo0(const Sprite* spr, const s2::RenderParams& params)
{
	sl::ShaderMgr::Instance()->GetShader()->Commit();

	DTexC1 *fbo0 = DTex::Instance()->GetFbo0(), 
		   *fbo1 = DTex::Instance()->GetFbo1();

	DrawInit(spr, params, fbo0);

	RenderContextStack* rc = RenderContextStack::Instance();

	sm::vec2 ori_offset;
	float ori_scale;
	rc->GetModelView(ori_offset, ori_scale);

	int ori_width, ori_height;
	rc->GetProjection(ori_width, ori_height);

	rc->SetModelView(sm::vec2(0, 0), 1);
	int edge = fbo0->GetTextureSize();
	rc->SetProjection(edge, edge);
	GL::Viewport(0, 0, edge, edge);

	DrawInit(spr, params, fbo0);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::FILTER);

	const int TIMES = 9;
	for (int i = 0; i < TIMES; ++i) {
		DrawBetweenFBO(fbo0, fbo1, true, params.color);
		DrawBetweenFBO(fbo1, fbo0, false, params.color);
	}

	rc->SetModelView(ori_offset, ori_scale);
	rc->SetProjection(ori_width, ori_height);
	GL::Viewport(0, 0, ori_width, ori_height);
}

void SpriteGaussianBlur::DrawToScreen(DTexC1* fbo, const sm::vec2& offset)
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

	shader->Draw(&vertices[0].x, &texcoords[0].x, fbo->GetTextureId());

	shader->Commit();
}

void SpriteGaussianBlur::DrawInit(const Sprite* spr, const s2::RenderParams& params, DTexC1* fbo)
{
	fbo->Bind();
	fbo->Clear(0, -2, 2, 0);

	s2::RenderParams _params = params;
	const sm::vec2& offset = spr->GetPosition();
	_params.mt.Translate(-offset.x, -offset.y, 0);
	_params.set_shader = false;
	_params.root_spr = spr->GetCore();
	_params.shader.filter = s2::FM_NULL;

	s2::FilterMode filter = spr->GetShader().filter;
	const_cast<Sprite*>(spr)->GetShader().filter = s2::FM_NULL;
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	SpriteRenderer::Draw(spr, _params);
	const_cast<Sprite*>(spr)->GetShader().filter = filter;

	fbo->Unbind();
}

void SpriteGaussianBlur::DrawBetweenFBO(DTexC1* from, DTexC1* to, bool hori, const s2::RenderColor& col)
{
	to->Bind();
	to->Clear(0, -2, 2, 0);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
	shader->SetMode(hori ? sl::FM_GAUSSIAN_BLUR_HORI : sl::FM_GAUSSIAN_BLUR_VERT);
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

	shader->Draw(&vertices[0].x, &texcoords[0].x, from->GetTextureId());

	shader->Commit();

	to->Unbind();
}

}