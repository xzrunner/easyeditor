#include <gl/glew.h>

#include "SpriteRenderer.h"
#include "ShaderMgr.h"
#include "ScreenFBO.h"
#include "BlendShader.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "dataset/SpriteTools.h"

namespace d2d
{

SpriteRenderer* SpriteRenderer::m_instance = NULL;

SpriteRenderer::SpriteRenderer()
	: m_fbo(600, 600)
{
}

void SpriteRenderer::Draw(const ISprite* sprite, 
						  const d2d::Matrix& mt,
						  const Colorf& mul, 
						  const Colorf& add,
						  const Colorf& r_trans,
						  const Colorf& g_trans,
						  const Colorf& b_trans,
						  bool multi_draw) const
{
	if (!sprite->visiable)
		return;

	if (!multi_draw || sprite->GetBlendMode() == BM_NORMAL) {
		DrawImpl(sprite, mt, mul, add, r_trans, g_trans, b_trans);
	} else {
 		ShaderMgr* mgr = ShaderMgr::Instance();
		FBO& scr_fbo = ScreenFBO::Instance()->GetFBO();
 
 		// 1. draw spr to tmp
		mgr->Commit();

 		mgr->sprite();
 		mgr->SetFBO(m_fbo.GetFboID());
 
 		glClearColor(1, 0, 0, 1);
 		glClear(GL_COLOR_BUFFER_BIT);
 
 		Vector offset;
 		float scale;
 		mgr->GetModelView(offset, scale);

 		mgr->SetModelView(Vector(0, 0), 1);
		mgr->SetProjection(m_fbo.GetWidth(), m_fbo.GetHeight());
		glViewport(0, 0, m_fbo.GetWidth(), m_fbo.GetHeight());

 		static_cast<BlendShader*>(mgr->GetSpriteShader())->SetBaseTexID(scr_fbo.GetTexID());
		Draw(&sprite->getSymbol());
		sprite->getSymbol().draw(Matrix(), Colorf(1, 1, 1, 1), Colorf(0, 0, 0, 0), 
			Colorf(1, 0, 0, 0), Colorf(0, 1, 0, 0), Colorf(0, 0, 1, 0), sprite);
 
		mgr->Commit();
 
 		mgr->SetModelView(offset, scale);
 		mgr->SetProjection(scr_fbo.GetWidth(), scr_fbo.GetHeight());
		glViewport(0, 0, scr_fbo.GetWidth(), scr_fbo.GetHeight());

 		// 2. draw tmp to screen fbo
 		mgr->sprite();
//		mgr->SetFBO(0);
		mgr->SetFBO(scr_fbo.GetFboID());

 		const d2d::Rect& r_dst = sprite->GetRect();
		float xmin = r_dst.xMin, xmax = r_dst.xMax;
		float ymin = r_dst.yMin, ymax = r_dst.yMax;
 		const float VERTICES[] = { xmin, ymin, xmin, ymax, xmax, ymax, xmax, ymin };
 
		d2d::Rect r_src = sprite->getSymbol().getSize();
		float txmin = r_src.xMin / m_fbo.GetWidth() + 0.5f,
			txmax = r_src.xMax / m_fbo.GetWidth() + 0.5f;
		float tymin = r_src.yMin / m_fbo.GetHeight() + 0.5f,
			tymax = r_src.yMax / m_fbo.GetHeight() + 0.5f;
 		const float TEXCOORDS[] = { txmin, tymin, txmin, tymax, txmax, tymax, txmax, tymin };
 		
		const float vertices[] = { 
			xmin, ymin, txmin, tymin,
			xmin, ymax, txmin, tymax, 
			xmax, ymax, txmax, tymax,
			xmax, ymin, txmax, tymin };

		mgr->Draw(vertices, m_fbo.GetTexID());

		mgr->Commit();
	}
}

void SpriteRenderer::Draw(const ISymbol* symbol, 
						  const d2d::Matrix& mt,
						  const Vector& pos, 
						  float angle/* = 0.0f*/, 
						  float xScale/* = 1.0f*/, 
						  float yScale/* = 1.0f*/, 
						  float xShear/* = 0.0f*/, 
						  float yShear/* = 0.0f*/, 
						  const Colorf& mul /*= Colorf(1,1,1,1)*/,
						  const Colorf& add /*= Colorf(0,0,0,0)*/,
						  const Colorf& r_trans,
						  const Colorf& g_trans,
						  const Colorf& b_trans) const
{
	Matrix t;
	t.setTransformation(pos.x, pos.y, angle, xScale, yScale, 0, 0, xShear, yShear);
	t = mt * t;
	symbol->draw(t, mul, add, r_trans, g_trans, b_trans);
}

void SpriteRenderer::DrawImpl(const ISprite* sprite, 
							  const d2d::Matrix& mt,
							  const Colorf& mul, 
							  const Colorf& add,
							  const Colorf& r_trans,
							  const Colorf& g_trans,
							  const Colorf& b_trans) const
{
	Matrix t;
	sprite->GetTransMatrix(t);
	t = mt * t;

	Colorf _mul = cMul(sprite->multiCol, mul),
		_add = cAdd(sprite->addCol, add);

	Colorf _r_trans, _g_trans, _b_trans;

	_r_trans.r = sprite->r_trans.r * r_trans.r + sprite->r_trans.g * g_trans.r + sprite->r_trans.b * b_trans.r;
	_r_trans.g = sprite->r_trans.r * r_trans.g + sprite->r_trans.g * g_trans.g + sprite->r_trans.b * b_trans.g;
	_r_trans.b = sprite->r_trans.r * r_trans.b + sprite->r_trans.g * g_trans.b + sprite->r_trans.b * b_trans.b;

	_g_trans.r = sprite->g_trans.r * r_trans.r + sprite->g_trans.g * g_trans.r + sprite->g_trans.b * b_trans.r;
	_g_trans.g = sprite->g_trans.r * r_trans.g + sprite->g_trans.g * g_trans.g + sprite->g_trans.b * b_trans.g;
	_g_trans.b = sprite->g_trans.r * r_trans.b + sprite->g_trans.g * g_trans.b + sprite->g_trans.b * b_trans.b;

	_b_trans.r = sprite->b_trans.r * r_trans.r + sprite->b_trans.g * g_trans.r + sprite->b_trans.b * b_trans.r;
	_b_trans.g = sprite->b_trans.r * r_trans.g + sprite->b_trans.g * g_trans.g + sprite->b_trans.b * b_trans.g;
	_b_trans.b = sprite->b_trans.r * r_trans.b + sprite->b_trans.g * g_trans.b + sprite->b_trans.b * b_trans.b;

	sprite->getSymbol().draw(t, _mul, _add, _r_trans, _g_trans, _b_trans, sprite);

	SpriteTools::DrawName(sprite, t);
}

SpriteRenderer* SpriteRenderer::Instance()
{
	if (!m_instance) {
		m_instance = new SpriteRenderer();
	}
	return m_instance;
}

} // d2d