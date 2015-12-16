#include "SpriteBlend.h"
#include "BlendShader.h"
#include "ShaderMgr.h"
#include "SpriteRenderer.h"
#include "ScreenCache.h"

#include "dataset/ISprite.h"
#include "dataset/ImageSymbol.h"

#include <dtex_facade.h>

namespace d2d
{

SpriteBlend* SpriteBlend::m_instance = NULL;

SpriteBlend* SpriteBlend::Instance()
{
	if (!m_instance) {
		m_instance = new SpriteBlend();
	}
	return m_instance;
}

SpriteBlend::SpriteBlend()
{
}

void SpriteBlend::Draw(const ISprite* sprite, const Matrix& mt) const
{
	assert(sprite->GetBlendMode() != BM_NORMAL);

	ShaderMgr* mgr = ShaderMgr::Instance();

 	dtexf_c1_clear();
 	dtexf_c1_bind();

	mgr->SpriteBlend();
	mgr->SetBlendMode(sprite->GetBlendMode());

	BlendShader* blend_shader = mgr->GetBlendShader();
	blend_shader->SetBaseTexID(ScreenCache::Instance()->GetTexID());

	SpriteRenderer::Instance()->DrawWithoutBlend(sprite, mt);

	mgr->Flush();

	dtexf_c1_unbind();

	mgr->SetSpriteShader(0);
}

}