#include "DrawNode.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "RenderFilter.h"
#include "DrawBlend.h"
#include "RFGaussianBlur.h"
#include "RFEdgeDetection.h"
#include "DrawGaussianBlur.h"
#include "DrawOuterGlow.h"
#include "RFOuterGlow.h"

#include <shaderlab.h>

namespace s2
{

static void (*AFTER_SPR)(const Sprite*, const RenderParams&);

void DrawNode::InitCB(void (*after_spr)(const Sprite*, const RenderParams&))
{
	AFTER_SPR = after_spr;
}

void DrawNode::Draw(const Sprite* spr, const RenderParams& params)
{
	if (!spr->IsVisible()) {
		return;
	}

	RenderShader rs = spr->Shader() * params.shader;
	RenderCamera rc = spr->Camera() * params.camera;

	sl::RenderContext* ctx = sl::ShaderMgr::Instance()->GetContext();
	switch (rs.fast_blend)
	{
	case FBM_NULL:
		ctx->SetBlend(2, 6);		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		ctx->SetBlendEquation(0);	// BLEND_FUNC_ADD
		break;
	case FBM_ADD:
		ctx->SetBlend(2, 2);		// BLEND_GL_ONE, BLEND_GL_ONE
		ctx->SetBlendEquation(0);	// BLEND_FUNC_ADD
		break;
	case FBM_SUBTRACT:
		ctx->SetBlend(2, 6);		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		ctx->SetBlendEquation(1);	// BLEND_FUNC_SUBTRACT
		break;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (rs.blend != BM_NULL) {
// 		const Camera* cam = CameraMgr::Instance()->GetCamera();
// 		if (cam->Type() == "ortho") {
			DrawBlend::Draw(spr, params.mt);
//		}
	} else if (rs.filter && rs.filter->GetMode() != FM_NULL) {
		RenderParams t = params;
		t.shader.filter = rs.filter;
		t.camera = rc;
		if (rs.filter->GetMode() == FM_GAUSSIAN_BLUR) {
 			int itrs = static_cast<RFGaussianBlur*>(rs.filter)->GetIterations();
 			DrawGaussianBlur::Draw(spr, t, itrs);
		} else if (rs.filter->GetMode() == FM_OUTER_GLOW) {
			int itrs = static_cast<RFOuterGlow*>(rs.filter)->GetIterations();
			DrawOuterGlow::Draw(spr, t, itrs);
		} else {
			if (params.set_shader) {
				mgr->SetShader(sl::FILTER);
			}
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
			shader->SetMode(sl::FILTER_MODE(rs.filter->GetMode()));
			switch (rs.filter->GetMode())
			{
			case FM_EDGE_DETECTION:
				{
					RFEdgeDetection* ed = static_cast<RFEdgeDetection*>(rs.filter);
					sl::EdgeDetectProg* prog = static_cast<sl::EdgeDetectProg*>(shader->GetProgram(sl::FM_EDGE_DETECTION));
					prog->SetBlend(ed->GetBlend());
				}
				break;
			}
			DrawSpr(spr, t);
		}
	} else {
		if (params.set_shader) {
			mgr->SetShader(sl::SPRITE2);
		}
		RenderParams t = params;
		t.camera = rc;
		DrawSpr(spr, t);
	}
}

void DrawNode::Draw(const Symbol* sym, const RenderParams& params, 
					const sm::vec2& pos, float angle, 
					const sm::vec2& scale, const sm::vec2& shear)
{
	sm::mat4 mt;
	mt.SetTransformation(pos.x, pos.y, angle, scale.x, scale.y, 0, 0, shear.x, shear.y);
	mt = mt * params.mt;

	RenderParams t = params;
	t.mt = mt;

	if (t.shader.blend != BM_NULL) {
		;
	} else if (t.shader.filter && t.shader.filter->GetMode() != FM_NULL) {
		;
	} else {
		if (t.set_shader) {
			sl::ShaderMgr::Instance()->SetShader(sl::SPRITE2);
		}
	}

	sym->Draw(t);
}

void DrawNode::DrawSpr(const Sprite* spr, const RenderParams& params)
{
	spr->GetSymbol()->Draw(params, spr);
	if (AFTER_SPR) {
		AFTER_SPR(spr, params);
	}
}

}