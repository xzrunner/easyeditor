#include "DrawBlend.h"
#include "RenderCtxStack.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "RenderParams.h"
#include "DrawNode.h"

#include <shaderlab.h>
#include <dtex_facade.h>
#include <SM_Rect.h>

#include <assert.h>

namespace s2
{

void DrawBlend::Draw(const Sprite* spr, const sm::mat4& mt)
{
	assert(spr->Shader().blend != BM_NULL);

	sl::ShaderMgr::Instance()->GetShader()->Commit();

	int edge = dtexf_t0_get_texture_size();
	RenderCtx ctx;
	ctx.mv_offset.Set(0, 0);
	ctx.mv_scale = 1;
	ctx.proj_width = ctx.proj_height = edge;
	RenderCtxStack::Instance()->Push(ctx);

	DrawSprToTmp(spr, mt);

	RenderCtxStack::Instance()->Pop();

	DrawTmpToScreen(spr, mt);
}

void DrawBlend::DrawSprToTmp(const Sprite* spr, const sm::mat4& mt)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::BlendShader* shader = static_cast<sl::BlendShader*>(mgr->GetShader(sl::BLEND));

	dtexf_t0_bind();
	dtexf_t0_clear(0, -2, 2, 0);

	mgr->SetShader(sl::BLEND);
	BlendMode mode = spr->Shader().blend;
	shader->SetMode(mode);

	const_cast<Sprite*>(spr)->Shader().blend = BM_NULL;
	RenderParams params;
	params.mt = mt;
	params.set_shader = false;
	params.vertex_offset = - (mt * spr->GetPosition());
	DrawNode::Draw(spr, params);
	const_cast<Sprite*>(spr)->Shader().blend = mode;

	shader->Commit();

	dtexf_t0_unbind();
}

void DrawBlend::DrawTmpToScreen(const Sprite* spr, const sm::mat4& mt)
{
	sm::mat4 t = spr->GetTransMatrix() * mt;
	sm::rect r = spr->GetSymbol()->GetBounding();

	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = t * vertices[i];
	}

	sm::vec2 vertex_offset = - (mt * spr->GetPosition());

	sm::vec2 texcoords[4];
	int edge = dtexf_t0_get_texture_size();
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i] + vertex_offset;
		texcoords[i].x = texcoords[i].x / edge + 0.5f;
		texcoords[i].y = texcoords[i].y / edge + 0.5f;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
	shader->Draw(&vertices[0].x, &texcoords[0].x, dtexf_t0_get_texture_id());
}

}