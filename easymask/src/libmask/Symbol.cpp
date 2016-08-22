#include "Symbol.h"
#include "FileIO.h"

#include <ee/SpriteRenderer.h>
#include <ee/Sprite.h>
#include <ee/Math2D.h>
#include <ee/RenderContextStack.h>
#include <ee/ImageSprite.h>
#include <ee/Image.h>
#include <ee/EE_DTex.h>

#include <easyparticle3d.h>
#include <easycomplex.h>

#include <dtex_facade.h>
#include <shaderlab.h>

#include <assert.h>

namespace emask
{

Symbol::Symbol()
	: m_base(NULL)
	, m_mask(NULL)
{
}

Symbol::~Symbol()
{
	if (m_base) {
		m_base->RemoveReference();
	}
	if (m_mask) {
		m_mask->RemoveReference();
	}
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}
	if (m_base && m_mask) {
		DrawImpl(p);
	} else {
		if (m_base) {
			ee::SpriteRenderer::Instance()->Draw(m_base, p);
		} else if (m_mask) {
			ee::SpriteRenderer::Instance()->Draw(m_mask, p);
		}
	}
}

sm::rect Symbol::GetBounding(const s2::Sprite* spr) const
{
	if (m_mask) {
		return m_mask->GetBounding();
	} else {
		return sm::rect(sm::vec2(0, 0), 100, 100);
	}
}

void Symbol::Update(const s2::RenderParams& params, float dt)
{
	UpdateP3DSymbol(const_cast<ee::Symbol*>(m_base), params, dt);
	UpdateP3DSymbol(const_cast<ee::Symbol*>(m_mask), params, dt);
}

void Symbol::SetSymbol(const ee::Symbol* sym, bool is_base)
{
	cu::RefCountObjAssign(is_base ? m_base: m_mask, sym);
}

void Symbol::LoadResources()
{
	FileIO::Load(m_filepath.c_str(), this);
}

void Symbol::DrawImpl(const s2::RenderParams& params) const
{
	sl::ShaderMgr::Instance()->GetShader()->Commit();

	ee::RenderContextStack* rc = ee::RenderContextStack::Instance();

	sm::vec2 ori_offset;
	float ori_scale;
	rc->GetModelView(ori_offset, ori_scale);

	int ori_width, ori_height;
	rc->GetProjection(ori_width, ori_height);

	rc->SetModelView(sm::vec2(0, 0), 1);
	int edge = dtexf_t0_get_texture_size();
	rc->SetProjection(edge, edge);
	sl::ShaderMgr::Instance()->GetContext()->SetViewport(0, 0, edge, edge);

	DrawBaseToFbo0(params.color);
	DrawMaskToFbo1();

	rc->SetModelView(ori_offset, ori_scale);
	rc->SetProjection(ori_width, ori_height);
	sl::ShaderMgr::Instance()->GetContext()->SetViewport(0, 0, ori_width, ori_height);

	DrawMashFromFbo(params.mt);
}

void Symbol::DrawBaseToFbo0(const s2::RenderColor& rc) const
{
	dtexf_t0_bind();
	dtexf_t0_clear(0, -2, 2, 0);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();

	s2::RenderParams params;
	params.set_shader = false;
	params.color = rc;
	ee::SpriteRenderer::Instance()->Draw(m_base, params);

	shader->Commit();

	dtexf_t0_unbind();
}

void Symbol::DrawMaskToFbo1() const
{
	dtexf_t1_bind();
	dtexf_t1_clear(0, -2, 2, 0);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();

	s2::RenderParams params;
	params.set_shader = false;
	ee::SpriteRenderer::Instance()->Draw(m_mask, params);

	shader->Commit();

	dtexf_t1_unbind();
}

void Symbol::DrawMashFromFbo(const sm::mat4& mt) const
{
	sm::vec2 vertices[4];
	sm::rect r = m_mask->GetBounding();
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);

	sm::vec2 texcoords[4];
	int edge0 = dtexf_t0_get_texture_size();
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i];
		texcoords[i].x = texcoords[i].x / edge0 + 0.5f;
		texcoords[i].y = texcoords[i].y / edge0 + 0.5f;
	}

	sm::vec2 texcoords_mask[4];
	int edge1 = dtexf_t1_get_texture_size();
	for (int i = 0; i < 4; ++i) {
		texcoords_mask[i] = vertices[i];
		texcoords_mask[i].x = texcoords_mask[i].x / edge1 + 0.5f;
		texcoords_mask[i].y = texcoords_mask[i].y / edge1 + 0.5f;
	}

	for (int i = 0; i < 4; ++i) {
		vertices[i] = mt * vertices[i];
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::MASK);
	sl::MaskShader* shader = static_cast<sl::MaskShader*>(mgr->GetShader());
	shader->Draw(&vertices[0].x, &texcoords[0].x, &texcoords_mask[0].x, dtexf_t0_get_texture_id(), dtexf_t1_get_texture_id());
}

void Symbol::UpdateP3DSymbol(ee::Symbol* sym, const s2::RenderParams& params, float dt)
{
	if (eparticle3d::Symbol* p3d = dynamic_cast<eparticle3d::Symbol*>(sym)) {
		p3d->Update(params, dt);
	} else if (ecomplex::Symbol* comp = dynamic_cast<ecomplex::Symbol*>(sym)) {
		const std::vector<s2::Sprite*>& children = comp->GetChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			if (eparticle3d::Sprite* p3d = dynamic_cast<eparticle3d::Sprite*>(children[i])) {
				p3d->Update(params, dt);
			}
		}
	}
}

}