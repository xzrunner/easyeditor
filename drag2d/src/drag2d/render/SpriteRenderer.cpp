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
	: m_fbo(400, 400)
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
 		mgr->sprite();
 		mgr->SetFBO(m_fbo.GetFboID());
 
 		glClearColor(1, 1, 0, 0);
 		glClear(GL_COLOR_BUFFER_BIT);
 
 		Vector offset;
 		float scale;
 		mgr->GetModelView(offset, scale);

 		mgr->SetModelView(Vector(0, 0), 1);
		mgr->SetProjection(m_fbo.GetWidth(), m_fbo.GetHeight());

 		static_cast<BlendShader*>(mgr->GetSpriteShader())->SetBaseTexID(scr_fbo.GetTexID());
		DrawImpl(sprite, Matrix(), mul, add, r_trans, g_trans, b_trans);
 
 		mgr->SetModelView(offset, scale);
 		mgr->SetProjection(scr_fbo.GetWidth(), scr_fbo.GetHeight());

 		// 2. draw tmp to screen fbo
 		mgr->sprite();
 		mgr->SetFBO(scr_fbo.GetFboID());
 
 		Vector c = Math::transVector(sprite->getPosition(), mt);
 		const d2d::Rect& r = sprite->GetRect();
 		float xmin = r.xMin + c.x,
 			xmax = r.xMax + c.x;
 		float ymin = r.yMin + c.y,
 			ymax = r.yMax + c.y;
 		const float VERTICES[] = { xmin, ymin, xmin, ymax, xmax, ymax, xmax, ymin };
 
		float txmin = r.xMin / m_fbo.GetWidth() + 0.5f,
			txmax = r.xMax / m_fbo.GetWidth() + 0.5f;
		float tymin = r.yMin / m_fbo.GetHeight() + 0.5f,
			tymax = r.yMax / m_fbo.GetHeight() + 0.5f;
 		const float TEXCOORDS[] = { txmin, tymin, txmin, tymax, txmax, tymax, txmax, tymin };
 		
 		//////////////////////////////////////////////////////////////////////////
 
 		mgr->SetTexture(m_fbo.GetTexID());
 //		glBindTexture(GL_TEXTURE_2D, m_fbo.GetTexID());
 
 		glEnableClientState(GL_VERTEX_ARRAY);
 		glVertexPointer(2, GL_FLOAT, 0, (const GLvoid*)VERTICES);
 
 		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 		glTexCoordPointer(2, GL_FLOAT, 0, (const GLvoid*)TEXCOORDS);
 
 		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
 
 		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 		glDisableClientState(GL_VERTEX_ARRAY);
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