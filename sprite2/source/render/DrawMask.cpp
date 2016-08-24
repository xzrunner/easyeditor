#include "DrawMask.h"
#include "RenderCtxStack.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "S2_Symbol.h"

#include <shaderlab.h>
#include <dtex_facade.h>
#include <SM_Rect.h>

namespace s2
{

void DrawMask::Draw(const Symbol* base, const Symbol* mask, const RenderParams& params)
{
	sl::ShaderMgr::Instance()->GetShader()->Commit();

	int edge = dtexf_t0_get_texture_size();
	RenderCtxStack::Instance()->Push(RenderCtx(edge, edge));

	DrawBaseToFbo0(base, params.color);
	DrawMaskToFbo1(mask);

	RenderCtxStack::Instance()->Pop();

	DrawMashFromFbo(mask, params.mt);
}

void DrawMask::DrawBaseToFbo0(const Symbol* base, const RenderColor& rc)
{
	dtexf_t0_bind();
	dtexf_t0_clear(0, -2, 2, 0);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();

	RenderParams params;
	params.set_shader = false;
	params.color = rc;
	DrawNode::Draw(base, params);

	shader->Commit();

	dtexf_t0_unbind();
}

void DrawMask::DrawMaskToFbo1(const Symbol* mask)
{
	dtexf_t1_bind();
	dtexf_t1_clear(0, -2, 2, 0);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();

	RenderParams params;
	params.set_shader = false;
	DrawNode::Draw(mask, params);

	shader->Commit();

	dtexf_t1_unbind();
}

void DrawMask::DrawMashFromFbo(const Symbol* mask, const sm::mat4& mt)
{
	sm::vec2 vertices[4];
	sm::rect r = mask->GetBounding();
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

}