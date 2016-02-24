#include "ArrangeSpriteImpl.h"
#include "StagePanel.h"
#include "Context.h"

#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Math2D.h>
#include <ee/DrawSelectedSpriteVisitor.h>
#include <ee/Symbol.h>

namespace etexpacker
{

ArrangeSpriteImpl::ArrangeSpriteImpl(StagePanel* editPanel, 
									 ee::PropertySettingPanel* property)
	: ee::ArrangeSpriteImpl(editPanel, editPanel->GetStageImpl(), editPanel, property, 
	ee::ArrangeSpriteConfig(false, false, false, false))
	, m_stage(editPanel)
{
}

void ArrangeSpriteImpl::OnMouseLeftUp(int x, int y)
{
	ee::ArrangeSpriteImpl::OnMouseLeftUp(x, y);
	m_selection->Traverse(FixCoordsVisitor());
}

void ArrangeSpriteImpl::OnMouseRightDown(int x, int y)
{
	ee::ArrangeSpriteImpl::OnMouseRightDown(x, y);
	OnMouseLeftDown(x, y);
}

void ArrangeSpriteImpl::OnMouseRightUp(int x, int y)
{
	if (m_selection->Size() == 1)
	{
		std::vector<ee::Sprite*> sprites;
		m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));

		const ee::Vector& pos = sprites[0]->GetPosition();
		float angle = sprites[0]->GetAngle();
		if (angle == 0)
			angle = ee::PI * 0.5f;
		else
			angle = 0;
		sprites[0]->SetTransform(pos, angle);
	}

	return OnMouseLeftUp(x, y);
}

void ArrangeSpriteImpl::OnDraw() const
{
	ee::ArrangeSpriteImpl::OnDraw(*m_stage->GetCamera());
	m_stage->TraverseSprites(
		ee::DrawSelectedSpriteVisitor(ee::Colorf(1.0f, 1.0f, 0.0f)),
		ee::DT_VISIBLE
		);
}

//////////////////////////////////////////////////////////////////////////
// class ArrangeSpriteImpl::FixCoordsVisitor
//////////////////////////////////////////////////////////////////////////

void ArrangeSpriteImpl::FixCoordsVisitor::
Visit(ee::Object* object, bool& next)
{
	ee::Sprite* sprite = static_cast<ee::Sprite*>(object);

	const ee::Vector& pos = sprite->GetPosition();

	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;
	float width, height;
	if (sprite->GetAngle() == 0)
	{
		width = sprite->GetSymbol().GetSize().Width() * s + p;
		height = sprite->GetSymbol().GetSize().Height() * s + p;
	}
	else
	{
		width = sprite->GetSymbol().GetSize().Height() * s + p;
		height = sprite->GetSymbol().GetSize().Width() * s + p;
	}

	ee::Vector leftTop;
	leftTop.x = pos.x - width * 0.5f;
	leftTop.y = pos.y - height * 0.5f;

	if (leftTop.x == std::floor(leftTop.x) && 
		leftTop.y == std::floor(leftTop.y))
	{
		next = true;
		return;
	}

	ee::Vector fixedCenter;
	fixedCenter.x = leftTop.x > 0 ? leftTop.x + 0.5f : leftTop.x - 0.5f;
	fixedCenter.y = leftTop.y > 0 ? leftTop.y + 0.5f : leftTop.y - 0.5f;
	fixedCenter.x = int(fixedCenter.x) + width * 0.5f;
	fixedCenter.y = int(fixedCenter.y) + height * 0.5f;

	sprite->SetTransform(fixedCenter, sprite->GetAngle());
	next = true;
}

}