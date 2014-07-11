#include "StagePanel.h"
#include "ArrangeSpriteOP.h"
#include "StageCanvas.h"
#include "BinaryTreeArrange.h"
#include "BinaryTreeNewArrange.h"
#include "RectBinArrange.h"
#include "Context.h"

using namespace epacker;

StagePanel::StagePanel(wxWindow* parent,
					   wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, SpritesPanelImpl(this, Context::Instance()->library)
{
	m_editOP = new ArrangeSpriteOP(this, Context::Instance()->property);
	m_canvas = new StageCanvas(this);

//	m_strategy = new BinaryTreeArrange();
//	m_strategy = new BinaryTreeNewArrange();

	m_strategy = new RectBinArrange();
}

void StagePanel::clear()
{
	EditPanel::clear();
	SpritesPanelImpl::clearSprites();
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::removeSprite(sprite);
	arrangeAllSprites(false);
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	fixCoords(sprite);

	d2d::SpritesPanelImpl::insertSprite(sprite);
	arrangeAllSprites(false);
}

void StagePanel::insertSpriteNoArrange(d2d::ISprite* sprite)
{
//	fixCoords(sprite);
	d2d::SpritesPanelImpl::insertSprite(sprite);
}

void StagePanel::arrangeAllSprites(bool bClearSelection)
{
	if (!Context::Instance()->auto_arrange) {
		return;
	}

	if (bClearSelection) m_spriteSelection->clear();

	std::vector<d2d::ImageSprite*> sprites;
	traverseSprites(d2d::FetchAllVisitor<d2d::ImageSprite>(sprites), d2d::e_editable);
	m_strategy->arrange(sprites);
}

void StagePanel::loadFromLibrary()
{
	clear();

	d2d::ISymbol* symbol = NULL;
	int index = 0;
	while (true)
	{
		symbol = Context::Instance()->library->getSymbol(index++);
		if (!symbol) 
			break;
		else 
		{
			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			d2d::SpritesPanelImpl::insertSprite(sprite);
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
	const d2d::Vector& pos = sprite->getPosition();

	const float s = Context::Instance()->scale;

	float width, height;
	if (sprite->getAngle() == 0)
	{
		width = sprite->getSymbol().getSize().xLength() * s;
		height = sprite->getSymbol().getSize().yLength() * s;
	}
	else
	{
		width = sprite->getSymbol().getSize().yLength() * s;
		height = sprite->getSymbol().getSize().xLength() * s;
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

		sprite->setTransform(fixedCenter, sprite->getAngle());
	}
}