#include "Symbol.h"
#include "Sprite.h"
#include "Node.h"
#include "Database.h"
#include "NodeType.h"
#include "LeafNode.h"

#include <SM_Test.h>
#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>
#include <gum/GUM_GTxt.h>

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

}