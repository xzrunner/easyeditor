#include <gl/glew.h>

#include "SpriteBlend.h"
#include "ShaderMgr.h"
#include "SpriteRenderer.h"
#include "ScreenCache.h"
#include "RenderContextStack.h"
#include "Sprite.h"
#include "ImageSymbol.h"
#include "GL.h"
#include "Math2D.h"
#include "BlendShader.h"
#include "EE_ShaderLab.h"

#include <dtex_facade.h>

#include <assert.h>

namespace ee
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

void SpriteBlend::Draw(const Sprite* sprite, const Matrix& mt) const
{
	assert(sprite->GetBlendMode() != BM_NORMAL);

	ShaderMgr::Instance()->Flush();

	RenderContextStack* rc = RenderContextStack::Instance();

	Vector ori_offset;
	float ori_scale;
	rc->GetModelView(ori_offset, ori_scale);

	int ori_width, ori_height;
	rc->GetProjection(ori_width, ori_height);

	rc->SetModelView(Vector(0, 0), 1);
	int edge = dtexf_c1_get_texture_size();
	rc->SetProjection(edge, edge);
	GL::Viewport(0, 0, edge, edge);

	DrawSprToTmp(sprite, mt);

	rc->SetModelView(ori_offset, ori_scale);
	rc->SetProjection(ori_width, ori_height);
	GL::Viewport(0, 0, ori_width, ori_height);

	DrawTmpToScreen(sprite, mt);
}

void SpriteBlend::DrawSprToTmp(const Sprite* sprite, const Matrix& mt) const
{
	ShaderMgr* mgr = ShaderMgr::Instance();

	dtexf_c1_clear();
	dtexf_c1_bind();

	mgr->Blend();
	BlendMode mode = sprite->GetBlendMode();
	mgr->SetBlendMode(mode);

	const_cast<Sprite*>(sprite)->SetBlendMode(BM_NORMAL);
	SpriteRenderer::Instance()->Draw(sprite, sprite, mt);
	const_cast<Sprite*>(sprite)->SetBlendMode(mode);

	mgr->Commit();
	ShaderLab::Instance()->Flush();

	dtexf_c1_unbind();
}

void SpriteBlend::DrawTmpToScreen(const Sprite* sprite, const Matrix& mt) const
{
	Matrix t;
	sprite->GetTransMatrix(t);
	t = mt * t;

	Rect r = sprite->GetSymbol().GetSize();

	Vector vertices[4];
	vertices[0] = Vector(r.xmin, r.ymin);
	vertices[1] = Vector(r.xmin, r.ymax);
	vertices[2] = Vector(r.xmax, r.ymax);
	vertices[3] = Vector(r.xmax, r.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = Math2D::TransVector(vertices[i], t);
	}

	Vector texcoords[4];
	int edge = dtexf_c1_get_texture_size();
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i] - sprite->GetPosition();
		texcoords[i].x = texcoords[i].x / edge + 0.5f;
		texcoords[i].y = texcoords[i].y / edge + 0.5f;
	}

	ShaderMgr* mgr = ShaderMgr::Instance();

	mgr->Sprite();
	mgr->SetSpriteColor(ColorTrans());
	mgr->Draw(vertices, texcoords, dtexf_c1_get_texture_id());
}

}