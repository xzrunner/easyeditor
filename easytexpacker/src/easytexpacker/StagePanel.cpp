#include "StagePanel.h"
#include "ArrangeSpriteOP.h"
#include "StageCanvas.h"
#include "BinaryTreeArrange.h"
#include "BinaryTreeNewArrange.h"
#include "MaxRectsBinaryArrange.h"
#include "RectBinArrange.h"
#include "Context.h"
#include "const.h"

using namespace etp;

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
}

void StagePanel::Clear()
{
	EditPanel::Clear();
	SpritesPanelImpl::ClearAllSprite();
}

bool StagePanel::InsertSprite(d2d::ISprite* sprite, int idx)
{
	fixCoords(sprite);

	bool ret = d2d::SpritesPanelImpl::InsertSprite(sprite);
	arrangeAllSprites(false);
	return ret;
}

bool StagePanel::RemoveSprite(d2d::ISprite* sprite)
{
	bool ret = d2d::SpritesPanelImpl::RemoveSprite(sprite);
	arrangeAllSprites(false);
	return ret;
}

void StagePanel::insertSpriteNoArrange(d2d::ISprite* sprite)
{
//	fixCoords(sprite);
	d2d::SpritesPanelImpl::InsertSprite(sprite);
}

void StagePanel::arrangeAllSprites(bool bClearSelection)
{
	if (!Context::Instance()->auto_arrange) {
		return;
	}

	if (bClearSelection) {
		m_sprite_selection->Clear();
	}

	std::vector<d2d::ImageSprite*> sprites;
	TraverseSprites(d2d::FetchAllVisitor<d2d::ImageSprite>(sprites), d2d::DT_EDITABLE);
	m_strategy->arrange(sprites);
}

void StagePanel::loadFromLibrary()
{
	Clear();

	d2d::ISymbol* symbol = NULL;
	int index = 0;
	while (true)
	{
		symbol = Context::Instance()->library->GetSymbol(index++);
		if (!symbol) 
			break;
		else 
		{
			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			d2d::SpritesPanelImpl::InsertSprite(sprite);
		}
	}

	arrangeAllSprites(true);
}

int StagePanel::GetTextureAccount() const
{
	return m_strategy->GetTextureAccount();
}

void StagePanel::fixCoords(d2d::ISprite* sprite)
{
	const d2d::Vector& pos = sprite->GetPosition();

	const float s = Context::Instance()->scale;

	float width, height;
	if (sprite->GetAngle() == 0)
	{
		width = sprite->GetSymbol().GetSize().xLength() * s;
		height = sprite->GetSymbol().GetSize().yLength() * s;
	}
	else
	{
		width = sprite->GetSymbol().GetSize().yLength() * s;
		height = sprite->GetSymbol().GetSize().xLength() * s;
	}

	d2d::Vector leftTop;
	leftTop.x = pos.x - width * 0.5f;
	leftTop.y = pos.y - height * 0.5f;

	if (leftTop.x != std::floor(leftTop.x) ||
		leftTop.y != std::floor(leftTop.y))
	{
		d2d::Vector fixedCenter;
		fixedCenter.x = leftTop.x > 0 ? leftTop.x + 0.5f : leftTop.x - 0.5f;
		fixedCenter.y = leftTop.y > 0 ? leftTop.y + 0.5f : leftTop.y - 0.5f;
		fixedCenter.x = int(fixedCenter.x) + width * 0.5f;
		fixedCenter.y = int(fixedCenter.y) + height * 0.5f;

		sprite->SetTransform(fixedCenter, sprite->GetAngle());
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