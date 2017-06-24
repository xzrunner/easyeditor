#include "Symbol.h"
#include "Sprite.h"
#include "Node.h"
#include "Database.h"
#include "NodeType.h"
#include "LeafNode.h"

#include <SM_Test.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>
#include <gum/GUM_GTxt.h>
#include <gum/Image.h>

#include <assert.h>

namespace edb
{

static const int TYPE = 2000;
static const int WIDTH = 250;
static const int HEIGHT = 50;
static const int BTN_RADIUS = 5;

Symbol::Symbol(const Database* db)
	: m_db(db)
{
}

Symbol::~Symbol()
{
}

int Symbol::Type() const
{
	return TYPE;
}

s2::RenderReturn Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	assert(spr);

	const Sprite* db_spr = dynamic_cast<const Sprite*>(spr);

	int id = db_spr->GetID();
	const Node* node = m_db->Fetch(id);
	assert(node);

	if (node->Type() == NODE_INDEX) {
		return s2::RENDER_NO_DATA;
	}

	const S2_MAT& mt = spr->GetLocalMat();

	s2::RVG::SetColor(s2::Color(127, 127, 0, 255));
	DrawBG(mt);

	const LeafNode* leaf = static_cast<const LeafNode*>(node);
	gum::GTxt::Instance()->Draw(mt, leaf->GetPath());

	bool in = !db_spr->IsClosed(true);
	bool out = !db_spr->IsClosed(false);

	s2::RVG::LineWidth(3);
	s2::RVG::SetColor(s2::Color(127, 0, 0, 255));
	DrawBtn(mt, sm::vec2(-WIDTH * 0.5f + BTN_RADIUS, 0), !in);
	DrawBtn(mt, sm::vec2(WIDTH * 0.5f - BTN_RADIUS, 0), !out);

	if (const gum::Image* ss = db_spr->GetSnapshoot()) {
		DrawSnapshoot(mt, ss);
	}

	if (!in && !out) {
		return s2::RENDER_OK;
	}

// 	if (in)
// 	{
// 		const std::set<int>& nodes = leaf->GetNodes(true);
// 	}
// 	if (out)
// 	{
// 		const std::set<int>& nodes = leaf->GetNodes(false);
// 	}

	return s2::RENDER_OK;
}

void Symbol::OnSprPressed(Sprite* spr, const sm::vec2& pos)
{
	const S2_MAT& mt = spr->GetLocalMat();

	sm::vec2 c = mt * sm::vec2(-WIDTH * 0.5f + BTN_RADIUS, 0);
	if (sm::is_point_in_circle(pos, c, BTN_RADIUS)) {
		spr->SetClosed(true, !spr->IsClosed(true));
	}

	c = mt * sm::vec2(WIDTH * 0.5f - BTN_RADIUS, 0);
	if (sm::is_point_in_circle(pos, c, BTN_RADIUS)) {
		spr->SetClosed(false, !spr->IsClosed(false));
	}
}

sm::rect Symbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
{
	return sm::rect(WIDTH, HEIGHT);
}

void Symbol::DrawBG(const S2_MAT& mt)
{
	sm::vec2 min = mt * sm::vec2(-WIDTH * 0.5f + BTN_RADIUS * 2, -HEIGHT * 0.5f);
	sm::vec2 max = mt * sm::vec2( WIDTH * 0.5f - BTN_RADIUS * 2,  HEIGHT * 0.5f);
	s2::RVG::Rect(min, max, true);
}

void Symbol::DrawBtn(const S2_MAT& mt, const sm::vec2& offset, bool closed)
{
	sm::vec2 center = mt * offset;
	s2::RVG::Circle(center, BTN_RADIUS, false);
	if (closed) {
		s2::RVG::Cross(center, BTN_RADIUS, BTN_RADIUS);
	} else {
		s2::RVG::Line(center - sm::vec2(BTN_RADIUS, 0), center + sm::vec2(BTN_RADIUS, 0));
	}
}

void Symbol::DrawSnapshoot(const S2_MAT& mt, const gum::Image* img)
{
	sm::ivec2 sz = img->GetSize();
	float hw = sz.x * 0.5f,
		  hh = sz.y * 0.5f;

	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(-hw, -hh);
	vertices[1] = sm::vec2( hw, -hh);
	vertices[2] = sm::vec2( hw,  hh);
	vertices[3] = sm::vec2(-hw,  hh);
	for (int i = 0; i < 4; ++i) 
	{
		sm::vec2 pos = mt * vertices[i];
		vertices[i] = pos;
	}

	int tex_id = img->GetTexID();
	float texcoords[8];
	texcoords[0] = 0; texcoords[1] = 0;
	texcoords[2] = 1; texcoords[3] = 0;
	texcoords[4] = 1; texcoords[5] = 1;
	texcoords[6] = 0; texcoords[7] = 1;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->DrawQuad(&vertices[0].x, texcoords, tex_id);
}

}