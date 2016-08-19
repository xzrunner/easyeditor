#include "NodeRenderer.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "RenderFilter.h"

#include <shaderlab.h>

namespace s2
{

void NodeRenderer::Draw(const Sprite* spr, const RenderParams& params)
{
	if (spr->IsVisible()) {
		return;
	}

	RenderShader rs = spr->Shader() * params.shader;
	RenderCamera rc = spr->Camera() * params.camera;

	
}

void NodeRenderer::Draw(const Symbol* sym, const RenderParams& params, 
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

}