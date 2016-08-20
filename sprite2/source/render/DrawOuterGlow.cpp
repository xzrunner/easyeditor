#include "DrawOuterGlow.h"
#include "DrawGaussianBlur.h"
#include "S2_Sprite.h"
#include "RenderParams.h"
#include "FilterFactory.h"
#include "DrawNode.h"

#include <shaderlab.h>

namespace s2
{

void DrawOuterGlow::Draw(const Sprite* spr, const RenderParams& params, int iterations)
{
	DrawGaussianBlur::DrawToFbo0(spr, params, iterations);
	DrawGaussianBlur::DrawToScreen(true, spr->GetPosition());

	RenderParams _params = params;
	_params.set_shader = false;
	_params.shader.filter = FilterFactory::Instance()->GetTemp(FM_NULL);

	Sprite* spr_no_const = const_cast<Sprite*>(spr);

	RenderFilter* ori_filter = spr->Shader().filter;
	spr_no_const->Shader().filter = FilterFactory::Instance()->GetTemp(FM_NULL);

	RenderColor ori_color = spr->Color();
	spr_no_const->Color() = RenderColor();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	DrawNode::Draw(spr, _params);

	spr_no_const->Shader().filter = ori_filter;	
	spr_no_const->Color() = ori_color;
}

}