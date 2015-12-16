#include "SpriteBlend2.h"
#include "BlendShader.h"
#include "ShaderMgr.h"
#include "SpriteRenderer.h"
#include "ScreenCache.h"

#include "dataset/ISprite.h"
#include "dataset/ImageSymbol.h"

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

	//////////////////////////////////////////////////////////////////////////

	assert(sprite->GetBlendMode() != BM_NORMAL);

	ShaderMgr* mgr = ShaderMgr::Instance();
	mgr->SetSpriteShader(m_blend_idx);

	mgr->SetBlendMode(sprite->GetBlendMode());
	mgr->sprite();

	BlendShader* blend_shader = static_cast<BlendShader*>(mgr->GetSpriteShader());
	blend_shader->SetBaseTexID(ScreenCache::Instance()->GetTexID());

	const d2d::Rect& r_dst = sprite->GetRect();
	float xmin = r_dst.xMin, xmax = r_dst.xMax;
	float ymin = r_dst.yMin, ymax = r_dst.yMax;

	float txmin = 0, tymin = 0;
	float txmax = 1, tymax = 1;

	d2d::Rect r_src = sprite->GetSymbol().GetSize();
	int scr_w, scr_h;
	ScreenCache::Instance()->GetSize(scr_w, scr_h);
	float tb_hw = r_dst.xLength() / scr_w * 0.5f,
		  tb_hh = r_dst.yLength() / scr_h * 0.5f;
	float tbxmin = -tb_hw, tbxmax = tb_hw,
		  tbymin = -tb_hh, tbymax = tb_hh;

	const float vertices[] = { 
		xmin, ymin, txmin, tymin, tbxmin, tbymin,
		xmin, ymax, txmin, tymax, tbxmin, tbymax,
		xmax, ymax, txmax, tymax, tbxmax, tbymax,
		xmax, ymin, txmax, tymin, tbxmax, tbymin };
	const ImageSymbol& img_sym = dynamic_cast<const ImageSymbol&>(sprite->GetSymbol());
	blend_shader->DrawBlend(vertices, img_sym.GetTexID());

	mgr->Commit();

	mgr->SetSpriteShader(0);
}

}