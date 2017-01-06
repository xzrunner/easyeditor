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
		std::vector<ee::Sprite*> sprs;
		m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));

		const sm::vec2& pos = sprs[0]->GetPosition();
		float angle = sprs[0]->GetAngle();
		if (angle == 0)
			angle = SM_PI * 0.5f;
		else
			angle = 0;
		sprs[0]->SetPosition(pos);
		sprs[0]->SetAngle(angle);
	}

	return OnMouseLeftUp(x, y);
}

void ArrangeSpriteImpl::OnDraw(float cam_scale) const
{
	ee::ArrangeSpriteImpl::OnDraw(cam_scale);
	m_stage->TraverseSprites(
		ee::DrawSelectedSpriteVisitor(s2::Color(1.0f, 1.0f, 0.0f)),
		ee::DT_VISIBLE
		);
}

//////////////////////////////////////////////////////////////////////////
// class ArrangeSpriteImpl::FixCoordsVisitor
//////////////////////////////////////////////////////////////////////////

void ArrangeSpriteImpl::FixCoordsVisitor::
Visit(ee::Sprite* spr, bool& next)
{
	const sm::vec2& pos = spr->GetPosition();

	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;
	float width, height;
	if (spr->GetAngle() == 0)
	{
		width = spr->GetSymbol()->GetBounding().Width() * s + p;
		height = spr->GetSymbol()->GetBounding().Height() * s + p;
	}
	else
	{
		width = spr->GetSymbol()->GetBounding().Height() * s + p;
		height = spr->GetSymbol()->GetBounding().Width() * s + p;
	}

	sm::vec2 leftTop;
	leftTop.x = pos.x - width * 0.5f;
	leftTop.y = pos.y - height * 0.5f;

	if (leftTop.x == std::floor(leftTop.x) && 
		leftTop.y == std::floor(leftTop.y))
	{
		next = true;
		return;
	}

	sm::vec2 fixedCenter;
	fixedCenter.x = leftTop.x > 0 ? leftTop.x + 0.5f : leftTop.x - 0.5f;
	fixedCenter.y = leftTop.y > 0 ? leftTop.y + 0.5f : leftTop.y - 0.5f;
	fixedCenter.x = int(fixedCenter.x) + width * 0.5f;
	fixedCenter.y = int(fixedCenter.y) + height * 0.5f;

	spr->SetPosition(fixedCenter);
	next = true;
}

}