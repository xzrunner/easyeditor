#include "StagePanel.h"
#include "ArrangeSpriteOP.h"
#include "StageCanvas.h"
#include "BinaryTreeArrange.h"
#include "BinaryTreeNewArrange.h"
#include "MaxRectsBinaryArrange.h"
#include "RectBinArrange.h"
#include "Context.h"
#include "const.h"

using namespace etexpacker;

StagePanel::StagePanel(wxWindow* parent,
					   wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, SpritesPanelImpl(GetStageImpl(), Context::Instance()->library)
{
//	m_editOP = new ArrangeSpriteOP(this, Context::Instance()->property);
	b2Body* ground = CreateGround();
	SetEditOP(new ephysics::DragPhysicsOP(this, GetStageImpl(), m_world, ground));

	SetCanvas(new StageCanvas(this));

//	m_strategy = new BinaryTreeArrange();
//	m_strategy = new BinaryTreeNewArrange();

	m_strategy = new MaxRectsBinaryArrange();

//	m_strategy = new RectBinArrange();

	RegistSubject(ee::InsertSpriteSJ::Instance());
	RegistSubject(ee::RemoveSpriteSJ::Instance());
}

void StagePanel::insertSpriteNoArrange(ee::Sprite* sprite)
{
//	fixCoords(sprite);
	ee::InsertSpriteSJ::Instance()->Insert(sprite);
}

void StagePanel::arrangeAllSprites(bool bClearSelection)
{
	if (!Context::Instance()->auto_arrange) {
		return;
	}

	if (bClearSelection) {
		m_sprite_selection->Clear();
	}

	std::vector<ee::ImageSprite*> sprites;
	TraverseSprites(ee::FetchAllVisitor<ee::ImageSprite>(sprites), ee::DT_EDITABLE);
	m_strategy->arrange(sprites);
}

void StagePanel::loadFromLibrary()
{
	ee::Symbol* symbol = NULL;
	int index = 0;
	while (true)
	{
		symbol = Context::Instance()->library->GetSymbol(index++);
		if (!symbol) 
			break;
		else 
		{
			ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
			ee::InsertSpriteSJ::Instance()->Insert(sprite);
		}
	}

	arrangeAllSprites(true);
}

int StagePanel::GetTextureAccount() const
{
	return m_strategy->GetTextureAccount();
}

void StagePanel::fixCoords(ee::Sprite* sprite)
{
	const ee::Vector& pos = sprite->GetPosition();

	const float s = Context::Instance()->scale;

	float width, height;
	if (sprite->GetAngle() == 0)
	{
		width = sprite->GetSymbol().GetSize().Width() * s;
		height = sprite->GetSymbol().GetSize().Height() * s;
	}
	else
	{
		width = sprite->GetSymbol().GetSize().Height() * s;
		height = sprite->GetSymbol().GetSize().Width() * s;
	}

	ee::Vector leftTop;
	leftTop.x = pos.x - width * 0.5f;
	leftTop.y = pos.y - height * 0.5f;

	if (leftTop.x != std::floor(leftTop.x) ||
		leftTop.y != std::floor(leftTop.y))
	{
		ee::Vector fixedCenter;
		fixedCenter.x = leftTop.x > 0 ? leftTop.x + 0.5f : leftTop.x - 0.5f;
		fixedCenter.y = leftTop.y > 0 ? leftTop.y + 0.5f : leftTop.y - 0.5f;
		fixedCenter.x = int(fixedCenter.x) + width * 0.5f;
		fixedCenter.y = int(fixedCenter.y) + height * 0.5f;

		sprite->SetTransform(fixedCenter, sprite->GetAngle());
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
			fixCoords(p->spr);
			// todo
			// SpritesPanelImpl::OnNotify(sj_id, ud);
			arrangeAllSprites(false);
		}
		break;
	case ee::MSG_REMOVE_SPRITE:
		arrangeAllSprites(false);
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