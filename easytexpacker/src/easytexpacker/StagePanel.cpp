#include "StagePanel.h"
#include "ArrangeSpriteOP.h"
#include "StageCanvas.h"
#include "BinaryTreeArrange.h"
#include "BinaryTreeNewArrange.h"
#include "MaxRectsBinaryArrange.h"
#include "RectBinArrange.h"
#include "Context.h"
#include "const.h"

#include <ee/sprite_msg.h>
#include <ee/DragPhysicsOP.h>
#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteFactory.h>
#include <ee/LibraryPanel.h>
#include <ee/subject_id.h>
#include <ee/ImageSprite.h>

namespace etexpacker
{

StagePanel::StagePanel(wxWindow* parent,
					   wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, SpritesPanelImpl(GetStageImpl(), Context::Instance()->library)
{
//	m_editop = new ArrangeSpriteOP(this, Context::Instance()->property);
	b2Body* ground = CreateGround();
	SetEditOP(new ee::DragPhysicsOP(this, GetStageImpl(), m_world, ground));

	SetCanvas(new StageCanvas(this));

//	m_strategy = new BinaryTreeArrange();
//	m_strategy = new BinaryTreeNewArrange();

	m_strategy = new MaxRectsBinaryArrange();

//	m_strategy = new RectBinArrange();

	RegistSubject(ee::InsertSpriteSJ::Instance());
	RegistSubject(ee::RemoveSpriteSJ::Instance());
}

void StagePanel::InsertSpriteNoArrange(ee::Sprite* spr)
{
//	fixCoords(spr);
	ee::InsertSpriteSJ::Instance()->Insert(spr);
}

void StagePanel::ArrangeAllSprites(bool bClearSelection)
{
	if (!Context::Instance()->auto_arrange) {
		return;
	}

	if (bClearSelection) {
		m_sprite_selection->Clear();
	}

	std::vector<ee::ImageSprite*> sprs;
	TraverseSprites(ee::FetchAllVisitor<ee::ImageSprite>(sprs), ee::DT_EDITABLE);
	m_strategy->Arrange(sprs);
}

void StagePanel::LoadFromLibrary()
{
	ee::Symbol* sym = NULL;
	int index = 0;
	while (true)
	{
		sym = Context::Instance()->library->GetSymbol(index++);
		if (!sym) 
			break;
		else 
		{
			ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
			ee::InsertSpriteSJ::Instance()->Insert(spr);
		}
	}

	ArrangeAllSprites(true);
}

int StagePanel::GetTextureAccount() const
{
	return m_strategy->GetTextureAccount();
}

void StagePanel::FixCoords(ee::Sprite* spr)
{
	const sm::vec2& pos = spr->GetPosition();

	const float s = Context::Instance()->scale;

	float width, height;
	if (spr->GetAngle() == 0)
	{
		width = spr->GetSymbol()->GetBounding().Width() * s;
		height = spr->GetSymbol()->GetBounding().Height() * s;
	}
	else
	{
		width = spr->GetSymbol()->GetBounding().Height() * s;
		height = spr->GetSymbol()->GetBounding().Width() * s;
	}

	sm::vec2 leftTop;
	leftTop.x = pos.x - width * 0.5f;
	leftTop.y = pos.y - height * 0.5f;

	if (leftTop.x != std::floor(leftTop.x) ||
		leftTop.y != std::floor(leftTop.y))
	{
		sm::vec2 fixedCenter;
		fixedCenter.x = leftTop.x > 0 ? leftTop.x + 0.5f : leftTop.x - 0.5f;
		fixedCenter.y = leftTop.y > 0 ? leftTop.y + 0.5f : leftTop.y - 0.5f;
		fixedCenter.x = int(fixedCenter.x) + width * 0.5f;
		fixedCenter.y = int(fixedCenter.y) + height * 0.5f;

		spr->SetPosition(fixedCenter);
	}
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	SpritesPanelImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case ee::MSG_INSERT_SPRITE:
		{
			ee::InsertSpriteSJ::Params* p = (ee::InsertSpriteSJ::Params*)ud;
			FixCoords(p->spr);
			// todo
			// SpritesPanelImpl::OnNotify(sj_id, ud);
			ArrangeAllSprites(false);
		}
		break;
	case ee::MSG_REMOVE_SPRITE:
		ArrangeAllSprites(false);
		break;
	}
}

b2Body* StagePanel::CreateGround()
{
	b2Vec2 vertices[6];
	vertices[0].Set(-HALF_WIDTH * 5,  HALF_WIDTH * 5);
	vertices[1].Set(-HALF_WIDTH,  HALF_H_EIGHT);
	vertices[2].Set(-HALF_WIDTH, -HALF_H_EIGHT);
	vertices[3].Set( HALF_WIDTH, -HALF_H_EIGHT);
	vertices[4].Set( HALF_WIDTH,  HALF_H_EIGHT);
	vertices[5].Set( HALF_WIDTH * 5,  HALF_WIDTH * 5);

	b2BodyDef bd;
	bd.type = b2_staticBody;
	b2Body* ground = m_world->CreateBody(&bd);

	b2ChainShape shape;
	shape.CreateChain(&vertices[0], 6);

	b2FixtureDef fd;
	fd.shape = &shape;

	ground->CreateFixture(&fd);

	return ground;
}

}
