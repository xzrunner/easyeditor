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
	DrawSprToTmp(sprite, mt);
	DrawTmpToScreen(sprite, mt);
}

void SpriteBlend::DrawSprToTmp(const ISprite* sprite, const Matrix& mt) const
{
	ShaderMgr* mgr = ShaderMgr::Instance();

	dtexf_c1_clear();
	dtexf_c1_bind();

	mgr->SpriteBlend();
	mgr->SetBlendMode(sprite->GetBlendMode());

	BlendShader* blend_shader = mgr->GetBlendShader();
	blend_shader->SetBaseTexID(ScreenCache::Instance()->GetTexID());

	SpriteRenderer::Instance()->DrawWithoutBlend(sprite, sprite, mt);

	dtexf_c1_unbind();

	mgr->SetSpriteShader(0);
}

void SpriteBlend::DrawTmpToScreen(const ISprite* sprite, const Matrix& mt) const
{
	Matrix t;
	sprite->GetTransMatrix(t);
	t = mt * t;

	Rect r = sprite->GetSymbol().GetSize();

	Vector vertices[4], texcoords[4];
	vertices[0] = texcoords[0] = Vector(r.xMin, r.yMin);
	vertices[1] = texcoords[1] = Vector(r.xMin, r.yMax);
	vertices[2] = texcoords[2] = Vector(r.xMax, r.yMax);
	vertices[3] = texcoords[3] = Vector(r.xMax, r.yMin);

	for (int i = 0; i < 4; ++i) {
		vertices[i] = Math::transVector(vertices[i], t);
	}

	static const float EDGE = 1024;
	for (int i = 0; i < 4; ++i) {
		texcoords[i].x = texcoords[i].x / EDGE + 0.5f;
		texcoords[i].y = texcoords[i].y / EDGE + 0.5f;
	}

	ShaderMgr* mgr = ShaderMgr::Instance();
	mgr->sprite();
	mgr->Draw(vertices, texcoords, dtexf_c1_get_texture_id());
}

}