#include "ArrangeSpriteImpl.h"
#include "StagePanel.h"
#include "Context.h"

namespace epacker
{

ArrangeSpriteImpl::ArrangeSpriteImpl(StagePanel* editPanel, 
									 d2d::PropertySettingPanel* propertyPanel)
	: d2d::ArrangeSpriteImpl(editPanel, editPanel->GetStageImpl(), editPanel, propertyPanel, 
	NULL, d2d::ArrangeSpriteConfig(false, false, false, false))
	, m_stage(editPanel)
{
}

void ArrangeSpriteImpl::OnMouseLeftUp(int x, int y)
{
	d2d::ArrangeSpriteImpl::OnMouseLeftUp(x, y);
	m_selection->Traverse(FixCoordsVisitor());
}

void ArrangeSpriteImpl::OnMouseRightDown(int x, int y)
{
	d2d::ArrangeSpriteImpl::OnMouseRightDown(x, y);
	OnMouseLeftDown(x, y);
}

void ArrangeSpriteImpl::OnMouseRightUp(int x, int y)
{
	if (m_selection->Size() == 1)
	{
		std::vector<d2d::ISprite*> sprites;
		m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

		const d2d::Vector& pos = sprites[0]->GetPosition();
		float angle = sprites[0]->GetAngle();
		if (angle == 0)
			angle = d2d::PI * 0.5f;
		else
			angle = 0;
		sprites[0]->SetTransform(pos, angle);
	}

	return OnMouseLeftUp(x, y);
}

void ArrangeSpriteImpl::OnDraw() const
{
	d2d::ArrangeSpriteImpl::OnDraw(*m_stage->GetCamera());
	m_stage->TraverseSprites(
		d2d::DrawSelectedSpriteVisitor(d2d::Colorf(1.0f, 1.0f, 0.0f)),
		d2d::DT_VISIBLE
		);
}

//////////////////////////////////////////////////////////////////////////
// class ArrangeSpriteImpl::FixCoordsVisitor
//////////////////////////////////////////////////////////////////////////

void ArrangeSpriteImpl::FixCoordsVisitor::
Visit(d2d::Object* object, bool& bFetchNext)
{
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);

	const d2d::Vector& pos = sprite->GetPosition();

	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;
	float width, height;
	if (sprite->GetAngle() == 0)
	{
		width = sprite->GetSymbol().GetSize().xLength() * s + p;
		height = sprite->GetSymbol().GetSize().yLength() * s + p;
	}
	else
	{
		width = sprite->GetSymbol().GetSize().yLength() * s + p;
		height = sprite->GetSymbol().GetSize().xLength() * s + p;
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

	sprite->SetTransform(fixedCenter, sprite->GetAngle());
	bFetchNext = true;
}

}