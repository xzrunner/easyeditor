#include "ArrangeSpriteOP.h"
#include "StagePanel.h"
#include "Context.h"

using namespace epacker;

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* editPanel, 
								 d2d::PropertySettingPanel* propertyPanel,
								 d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(editPanel, editPanel, propertyPanel, callback, false, false)
{
	m_editPanel = editPanel;
}

bool ArrangeSpriteOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::onMouseLeftUp(x, y)) return true;

	m_selection->traverse(FixCoordsVisitor());

	return false;
}

bool ArrangeSpriteOP::onMouseRightDown(int x, int y)
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::onMouseRightDown(x, y)) return true;

	onMouseLeftDown(x, y);

	return false;
}

bool ArrangeSpriteOP::onMouseRightUp(int x, int y)
{
	if (m_selection->size() == 1)
	{
		std::vector<d2d::ISprite*> sprites;
		m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

		const d2d::Vector& pos = sprites[0]->getPosition();
		float angle = sprites[0]->getAngle();
		if (angle == 0)
			angle = d2d::PI * 0.5f;
		else
			angle = 0;
		sprites[0]->setTransform(pos, angle);
	}

	return onMouseLeftUp(x, y);
}

bool ArrangeSpriteOP::onDraw() const
{
	m_editPanel->traverseSprites(
		d2d::DrawSelectedSpriteVisitor(d2d::Colorf(1.0f, 1.0f, 0.0f)),
		d2d::e_visible
		);
	return d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::onDraw();
}

//////////////////////////////////////////////////////////////////////////
// class ArrangeSpriteOP::FixCoordsVisitor
//////////////////////////////////////////////////////////////////////////

void ArrangeSpriteOP::FixCoordsVisitor::
visit(d2d::Object* object, bool& bFetchNext)
{
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);

	const d2d::Vector& pos = sprite->getPosition();

	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;
	float width, height;
	if (sprite->getAngle() == 0)
	{
		width = sprite->getSymbol().getSize().xLength() * s + p;
		height = sprite->getSymbol().getSize().yLength() * s + p;
	}
	else
	{
		width = sprite->getSymbol().getSize().yLength() * s + p;
		height = sprite->getSymbol().getSize().xLength() * s + p;
	}

	d2d::Vector leftTop;
	leftTop.x = pos.x - width * 0.5f;
	leftTop.y = pos.y - height * 0.5f;

	if (leftTop.x == std::floor(leftTop.x) && 
		leftTop.y == std::floor(leftTop.y))
	{
		bFetchNext = true;
		return;
	}

	d2d::Vector fixedCenter;
	fixedCenter.x = leftTop.x > 0 ? leftTop.x + 0.5f : leftTop.x - 0.5f;
	fixedCenter.y = leftTop.y > 0 ? leftTop.y + 0.5f : leftTop.y - 0.5f;
	fixedCenter.x = int(fixedCenter.x) + width * 0.5f;
	fixedCenter.y = int(fixedCenter.y) + height * 0.5f;

	sprite->setTransform(fixedCenter, sprite->getAngle());
	bFetchNext = true;
}