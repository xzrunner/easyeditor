#include "SpriteRenderer.h"
#include "S2_Sprite.h"

namespace s2
{

void SpriteRenderer::Draw(const Sprite* spr, const RenderParams& params)
{
	if (spr->IsVisible()) {
		return;
	}

	RenderShader rs = spr->Shader() * params.shader;
	RenderCamera rc = spr->Camera() * params.camera;

	
}

void SpriteRenderer::Draw(const Symbol* sym, const RenderParams& params, 
						  const sm::vec2& pos, float angle, 
						  const sm::vec2& scale, const sm::vec2& shear)
{

}

}