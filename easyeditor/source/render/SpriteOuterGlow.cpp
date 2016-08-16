#include "SpriteOuterGlow.h"
#include "SpriteGaussianBlur.h"
#include "EE_DTex.h"
#include "Sprite.h"
#include "SpriteRenderer.h"

#include <shaderlab.h>
#include <sprite2/RenderParams.h>
#include <sprite2/FilterFactory.h>

#include <assert.h>

namespace ee
{

void SpriteOuterGlow::Draw(const Sprite* spr, const s2::RenderParams& params, int iterations)
{
	SpriteGaussianBlur::DrawToFbo0(spr, params, iterations);
	SpriteGaussianBlur::DrawToScreen(true, spr->GetPosition());

	s2::RenderParams _params = params;
	_params.set_shader = false;
	_params.shader.filter = s2::FilterFactory::Instance()->GetTemp(s2::FM_NULL);

	Sprite* spr_no_const = const_cast<Sprite*>(spr);

	s2::RenderFilter* ori_filter = spr->Shader().filter;
	spr_no_const->Shader().filter = s2::FilterFactory::Instance()->GetTemp(s2::FM_NULL);

	s2::RenderColor ori_color = spr->Color();
	spr_no_const->Color() = s2::RenderColor();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	SpriteRenderer::Draw(spr, _params);

	spr_no_const->Shader().filter = ori_filter;	
	spr_no_const->Color() = ori_color;
}

}