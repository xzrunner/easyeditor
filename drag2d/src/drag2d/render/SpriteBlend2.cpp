#include "SpriteBlend2.h"
#include "BlendShader.h"
#include "ShaderMgr.h"
#include "SpriteRenderer.h"

#include <dtex_facade.h>

namespace d2d
{

SpriteBlend2* SpriteBlend2::m_instance = NULL;

SpriteBlend2* SpriteBlend2::Instance()
{
	if (!m_instance) {
		m_instance = new SpriteBlend2();
	}
	return m_instance;
}

SpriteBlend2::SpriteBlend2()
{
	SpriteShader* blend_shader = new BlendShader;
	blend_shader->Load();
	m_blend_idx = ShaderMgr::Instance()->AddSpriteShader(blend_shader);
}

void SpriteBlend2::Draw(const ISprite* sprite, const Matrix& mt) const
{
// 	// draw spr to tmp
// 	dtexf_c1_clear();
// 	dtexf_c1_bind();
// 
// 	ShaderMgr* mgr = ShaderMgr::Instance();
// 	mgr->SetBlendMode(sprite->GetBlendMode());
// 
// 	BlendShader* blend_shader = static_cast<BlendShader*>(mgr->GetSpriteShader());
// 	blend_shader->SetBaseTexID(scr_fbo.GetTexID());
// 
// 	SpriteRenderer::Instance()->NormalDraw(sprite, mt);
// 
// 	dtexf_c1_unbind();
}

}