#include "Symbol.h"
#include "FileIO.h"

#include <ee/SpriteRenderer.h>
#include <ee/Sprite.h>
#include <ee/Math2D.h>
#include <ee/RenderContextStack.h>
#include <ee/GL.h>
#include <ee/BoundingBox.h>
#include <ee/ImageSprite.h>
#include <ee/Image.h>
#include <ee/EE_DTex.h>

#include <dtex_facade.h>
#include <shaderlab.h>

#include <assert.h>

namespace emask
{

Symbol::Symbol()
	: m_base_spr(NULL)
	, m_mask_spr(NULL)
{
}

Symbol::~Symbol()
{
	if (m_base_spr) {
		m_base_spr->Release();
	}
	if (m_mask_spr) {
		m_mask_spr->Release();
	}
}

void Symbol::Draw(const s2::RenderParams& params, const ee::Sprite* spr) const
{
	if (m_base_spr && m_mask_spr) {
		Draw(params.mt);
	} else {
		if (m_base_spr) {
			ee::SpriteRenderer::Draw(m_base_spr, params);
		} else if (m_mask_spr) {
			ee::SpriteRenderer::Draw(m_mask_spr, params);
		}		
	}
}

sm::rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	return m_bounding;
}

void Symbol::SetSprite(ee::Sprite* spr, bool is_base) 
{
	ee::obj_assign(is_base ? m_base_spr : m_mask_spr, spr);

	m_bounding.MakeEmpty();
	if (m_base_spr) {
		std::vector<sm::vec2> vertices;
		m_base_spr->GetBounding()->GetBoundPos(vertices);
		for (int j = 0, m = vertices.size(); j < m; ++j) {
			m_bounding.Combine(vertices[j]);
		}
	}
	if (m_mask_spr) {
		std::vector<sm::vec2> vertices;
		m_mask_spr->GetBounding()->GetBoundPos(vertices);
		for (int j = 0, m = vertices.size(); j < m; ++j) {
			m_bounding.Combine(vertices[j]);
		}
	}
}

void Symbol::LoadResources()
{
	FileIO::Load(m_filepath.c_str(), this);
}

void Symbol::Draw(const sm::mat4& mt) const
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
	ee::GL::Viewport(0, 0, edge, edge);

	DrawBaseToFbo0();
	DrawMaskToFbo1();

	rc->SetModelView(ori_offset, ori_scale);
	rc->SetProjection(ori_width, ori_height);
	ee::GL::Viewport(0, 0, ori_width, ori_height);

	DrawMashFromFbo(mt);
}

void Symbol::DrawBaseToFbo0() const
{
	dtexf_t0_bind();
	dtexf_t0_clear(0, -2, 2, 0);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::BlendShader* shader = static_cast<sl::BlendShader*>(mgr->GetShader(sl::SPRITE2));

	s2::RenderParams params;
	const sm::vec2& offset = m_base_spr->GetPosition();
	params.mt.Translate(-offset.x, -offset.y, 0);
	params.set_shader = false;
	params.root_spr = m_base_spr->GetCore();
	ee::SpriteRenderer::Draw(m_base_spr, params);

	shader->Commit();

	dtexf_t0_unbind();
}

void Symbol::DrawMaskToFbo1() const
{
	dtexf_t1_bind();
	dtexf_t1_clear(0, -2, 2, 0);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::BlendShader* shader = static_cast<sl::BlendShader*>(mgr->GetShader(sl::SPRITE2));

	s2::RenderParams params;
	const sm::vec2& offset = m_base_spr->GetPosition();
	params.mt.Translate(-offset.x, -offset.y, 0);
	params.set_shader = false;
	params.root_spr = m_mask_spr->GetCore();
	ee::SpriteRenderer::Draw(m_mask_spr, params);

	shader->Commit();

	dtexf_t1_unbind();
}

void Symbol::DrawMashFromFbo(const sm::mat4& mt) const
{
	sm::mat4 t;
	m_mask_spr->GetTransMatrix(t);

	sm::rect r = m_mask_spr->GetSymbol().GetSize();

	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = t * vertices[i];
	}

	sm::vec2 texcoords[4];
	const sm::vec2& offset = m_base_spr->GetPosition();
	int edge0 = dtexf_t0_get_texture_size();
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i] - offset;
		texcoords[i].x = texcoords[i].x / edge0 + 0.5f;
		texcoords[i].y = texcoords[i].y / edge0 + 0.5f;
	}

	sm::vec2 texcoords_mask[4];
	int edge1 = dtexf_t1_get_texture_size();
	for (int i = 0; i < 4; ++i) {
		texcoords_mask[i] = vertices[i] - offset;
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

}