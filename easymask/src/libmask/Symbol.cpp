#include "Symbol.h"
#include "FileIO.h"

#include <ee/SpriteRenderer.h>
#include <ee/Sprite.h>
#include <ee/Math2D.h>
#include <ee/RenderContextStack.h>
#include <ee/GL.h>
#include <ee/BoundingBox.h>

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
	int edge = dtexf_c1_get_texture_size();
	rc->SetProjection(edge, edge);
	ee::GL::Viewport(0, 0, edge, edge);

	DrawSprToTmp(mt);

	rc->SetModelView(ori_offset, ori_scale);
	rc->SetProjection(ori_width, ori_height);
	ee::GL::Viewport(0, 0, ori_width, ori_height);

	DrawTmpToScreen(mt);
}

void Symbol::DrawSprToTmp(const sm::mat4& mt) const
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::BlendShader* shader = static_cast<sl::BlendShader*>(mgr->GetShader(sl::BLEND));

	dtexf_c1_bind();
	dtexf_c1_clear(0, -2, 2, 0);

	mgr->SetShader(sl::MASK);

	s2::RenderParams params;
	params.mt = mt;
	params.set_shader = false;
	params.root_spr = m_base_spr->GetCore();
	ee::SpriteRenderer::Draw(m_base_spr, params);

	shader->Commit();

	dtexf_c1_unbind();
}

void Symbol::DrawTmpToScreen(const sm::mat4& mt) const
{
	sm::mat4 t;
	m_mask_spr->GetTransMatrix(t);
	t = t * mt;

	sm::rect r = m_mask_spr->GetSymbol().GetSize();

	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = ee::Math2D::TransVector(vertices[i], t);
	}

	sm::vec2 texcoords[4];
	int edge = dtexf_c1_get_texture_size();
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i] - m_mask_spr->GetPosition();
		texcoords[i].x = texcoords[i].x / edge + 0.5f;
		texcoords[i].y = texcoords[i].y / edge + 0.5f;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
	shader->Draw(&vertices[0].x, &texcoords[0].x, dtexf_c1_get_texture_id());
}

}