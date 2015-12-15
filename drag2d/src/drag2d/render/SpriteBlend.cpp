//#include "SpriteBlend.h"
//
//namespace d2d
//{
//
//SpriteBlend* SpriteBlend::m_instance = NULL;
//
//SpriteBlend* SpriteBlend::Instance()
//{
//	if (!m_instance) {
//		m_instance = new SpriteBlend();
//	}
//	return m_instance;
//}
//
//SpriteBlend::SpriteBlend()
//	: m_fbo(NULL)
//	, m_blend_idx(0)
//{
//}
//
//SpriteBlend::~SpriteBlend()
//{
//	delete m_fbo;
//}
//
//void SpriteBlend::Draw(const ISprite* sprite) const
//{
//	if (!m_fbo) {
//		InitBlendShader();
//	}
//
//	ShaderMgr* mgr = ShaderMgr::Instance();
//	mgr->SetSpriteShader(m_blend_idx);
//
//	DrawImplBlend(sprite);
//
//	mgr->SetSpriteShader(0);
//	mgr->sprite();
//	FBO& scr_fbo = ScreenFBO::Instance()->GetFBO();
//	mgr->SetFBO(scr_fbo.GetFboID());
//}
//
//void SpriteBlend::DrawImplBlend(const ISprite* sprite) const
//{
//	//	DrawUnderToTmp(sprite);
//	DrawSprToTmp(sprite);
//	DrawTmpToScreen(sprite);
//}
//
//void SpriteBlend::DrawUnderToTmp(const ISprite* sprite) const
//{
//	ShaderMgr* mgr = ShaderMgr::Instance();
//	FBO& scr_fbo = ScreenFBO::Instance()->GetFBO();
//
//	mgr->sprite();
//	mgr->SetFBO(m_fbo->GetFboID());
//
//	mgr->SetBlendMode(BM_NORMAL);
//
//	glClearColor(0, 0, 0, 1);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	// src 
//	Vector src[4];
//	Rect src_rect = sprite->GetRect();
//	int scr_w = scr_fbo.GetWidth(),
//		scr_h = scr_fbo.GetHeight();
//	src[0] = m_cam->transPosProjectToScreen(Vector(src_rect.xMin, src_rect.yMin), scr_w, scr_h);
//	src[1] = m_cam->transPosProjectToScreen(Vector(src_rect.xMax, src_rect.yMin), scr_w, scr_h);
//	src[2] = m_cam->transPosProjectToScreen(Vector(src_rect.xMax, src_rect.yMax), scr_w, scr_h);
//	src[3] = m_cam->transPosProjectToScreen(Vector(src_rect.xMin, src_rect.yMax), scr_w, scr_h);
//	for (int i = 0; i < 4; ++i) {
//		src[i].y = scr_h - 1 - src[i].y;
//		src[i].x /= scr_w;
//		src[i].y /= scr_h;
//		src[i].x = std::min(std::max(0.0f, src[i].x), 1.0f);
//		src[i].y = std::min(std::max(0.0f, src[i].y), 1.0f);
//	}
//
//	// dst
//	Vector dst[4];
//	Rect dst_rect = sprite->GetSymbol().GetSize();
//	dst[0] = Vector(dst_rect.xMin, dst_rect.yMin);
//	dst[1] = Vector(dst_rect.xMax, dst_rect.yMin);
//	dst[2] = Vector(dst_rect.xMax, dst_rect.yMax);
//	dst[3] = Vector(dst_rect.xMin, dst_rect.yMax);
//
//	RenderContextStack* ctx_stack = RenderContextStack::Instance();
//
//	Vector offset;
//	float scale;
//	ctx_stack->GetModelView(offset, scale);
//
//	ctx_stack->SetModelView(Vector(0, 0), 1);
//	Rect r = sprite->GetSymbol().GetSize();
//	ctx_stack->SetProjection(r.xLength(), r.yLength());
//	// glViewport no need, from DrawCallBatching?
//	//	glViewport(0, 0, r.xLength(), r.yLength());
//
//	BlendShader* blend_shader = static_cast<BlendShader*>(mgr->GetSpriteShader());
//	blend_shader->SetBaseTexID(scr_fbo.GetTexID());
//	blend_shader->DrawBlend(dst, src, src, scr_fbo.GetTexID());
//
//	mgr->Commit();
//
//	ctx_stack->SetModelView(offset, scale);
//	ctx_stack->SetProjection(scr_fbo.GetWidth(), scr_fbo.GetHeight());
//	// glViewport no need, from DrawCallBatching?
//	//	glViewport(0, 0, scr_fbo.GetWidth(), scr_fbo.GetHeight());
//}
//
//void SpriteBlend::DrawSprToTmp(const ISprite* sprite) const
//{
//	ShaderMgr* mgr = ShaderMgr::Instance();
//	FBO& scr_fbo = ScreenFBO::Instance()->GetFBO();
//
//	mgr->sprite();
//	mgr->SetFBO(m_fbo->GetFboID());
//
//	mgr->SetBlendMode(sprite->GetBlendMode());
//
//	glClearColor(0, 0, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	RenderContextStack* ctx_stack = RenderContextStack::Instance();
//
//	Vector offset;
//	float scale;
//	ctx_stack->GetModelView(offset, scale);
//
//	ctx_stack->SetModelView(Vector(0, 0), 1);
//	Rect r = sprite->GetSymbol().GetSize();
//	ctx_stack->SetProjection(r.xLength(), r.yLength());
//	// glViewport no need, from DrawCallBatching?
//	//	glViewport(0, 0, r.xLength(), r.yLength());
//
//	BlendShader* blend_shader = static_cast<BlendShader*>(mgr->GetSpriteShader());
//	blend_shader->SetBaseTexID(scr_fbo.GetTexID());
//	//	blend_shader->SetBaseTexID(m_fbo->GetTexID());
//
//	sprite->GetSymbol().Draw(Matrix(), sprite->multiCol, sprite->addCol, 
//		sprite->r_trans, sprite->g_trans, sprite->b_trans, sprite);
//
//	mgr->Commit();
//
//	ctx_stack->SetModelView(offset, scale);
//	ctx_stack->SetProjection(scr_fbo.GetWidth(), scr_fbo.GetHeight());
//	// glViewport no need, from DrawCallBatching?
//	//	glViewport(0, 0, scr_fbo.GetWidth(), scr_fbo.GetHeight());
//}
//
//void SpriteBlend::DrawTmpToScreen(const ISprite* sprite) const
//{
//	ShaderMgr* mgr = ShaderMgr::Instance();
//	FBO& scr_fbo = ScreenFBO::Instance()->GetFBO();
//
//	mgr->sprite();
//	mgr->SetFBO(0);
//	mgr->SetFBO(scr_fbo.GetFboID());
//
//	mgr->SetBlendMode(BM_NORMAL);
//
//	const d2d::Rect& r_dst = sprite->GetRect();
//	float xmin = r_dst.xMin, xmax = r_dst.xMax;
//	float ymin = r_dst.yMin, ymax = r_dst.yMax;
//
//	d2d::Rect r_src = sprite->GetSymbol().GetSize();
//	//  		float txmin = r_src.xMin / m_fbo->GetWidth() + 0.5f,
//	//  			txmax = r_src.xMax / m_fbo->GetWidth() + 0.5f;
//	//  		float tymin = r_src.yMin / m_fbo->GetHeight() + 0.5f,
//	//  			tymax = r_src.yMax / m_fbo->GetHeight() + 0.5f;
//	float txmin = 0, txmax = r_src.xLength() / m_fbo->GetWidth();
//	float tymin = 0, tymax = r_src.yLength() / m_fbo->GetHeight();
//	if (BlendShader* blend_shader = dynamic_cast<BlendShader*>(mgr->GetSpriteShader()))
//	{
//		const float vertices[] = { 
//			xmin, ymin, txmin, tymin, txmin, tymin,
//			xmin, ymax, txmin, tymax, txmin, tymax,
//			xmax, ymax, txmax, tymax, txmax, tymax,
//			xmax, ymin, txmax, tymin, txmax, tymin };
//			blend_shader->DrawBlend(vertices, m_fbo->GetTexID());
//	}
//	else
//	{
//		const float vertices[] = { 
//			xmin, ymin, txmin, tymin,
//			xmin, ymax, txmin, tymax,
//			xmax, ymax, txmax, tymax,
//			xmax, ymin, txmax, tymin};
//			mgr->Draw(vertices, m_fbo->GetTexID());
//	}
//
//	mgr->Commit();
//}
//
//void SpriteBlend::InitBlendShader() const
//{
//	m_fbo = new FBO(600, 600);	
//
//	d2d::SpriteShader* blend_shader = new d2d::BlendShader;
//	blend_shader->Load();
//	m_blend_idx = ShaderMgr::Instance()->AddSpriteShader(blend_shader);
//}
//
//}