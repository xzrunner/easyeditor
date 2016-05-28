#include "SpriteOuterGlow.h"
#include "SpriteGaussianBlur.h"
#include "EE_DTex.h"
#include "Sprite.h"
#include "SpriteRenderer.h"

#include <shaderlab.h>
#include <sprite2/RenderParams.h>

namespace ee
{

void SpriteOuterGlow::Draw(const Sprite* spr, const s2::RenderParams& params)
{
	SpriteGaussianBlur::DrawToFbo0(spr, params);
	SpriteGaussianBlur::DrawToScreen(DTex::Instance()->GetFbo0(), spr->GetPosition());

	s2::RenderParams _params = params;
	_params.set_shader = false;
	_params.root_spr = spr->GetCore();
	_params.shader.filter = s2::FM_NULL;

	Sprite* spr_no_const = const_cast<Sprite*>(spr);

	s2::FilterMode ori_filter = spr->GetShader().filter;
	spr_no_const->GetShader().filter = s2::FM_NULL;

	s2::RenderColor ori_color = spr->GetColor();
	spr_no_const->GetColor() = s2::RenderColor();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	SpriteRenderer::Draw(spr, _params);

	spr_no_const->GetShader().filter = ori_filter;	
	spr_no_const->GetColor() = ori_color;
}

}