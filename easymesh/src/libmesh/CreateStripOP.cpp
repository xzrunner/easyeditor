#include "CreateStripOP.h"
#include "StagePanel.h"
#include "EditShape.h"
#include "Symbol.h"

namespace emesh
{

CreateStripOP::CreateStripOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true, false)
	, m_stage(stage)
	, m_selected(NULL)
{
	m_last_right.setInvalid();
}

bool CreateStripOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y))
		return true;

	if (EditShape* shape = static_cast<EditShape*>(m_stage->GetShape()))
	{
		d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
		m_selected = shape->FindNode(pos);
	}

	return false;
}

bool CreateStripOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftUp(x, y))
		return true;

	if (m_selected) {
		return false;
	}

	if (EditShape* shape = static_cast<EditShape*>(m_stage->GetShape()))
	{
		d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
		shape->InsertNode(pos);
		m_stage->SetCanvasDirty();
	}

	return false;
}

bool CreateStripOP::OnMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseRightDown(x, y))
		return true;

	if (EditShape* shape = static_cast<EditShape*>(m_stage->GetShape()))
	{
		d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
		shape->RemoveNode(pos);
		m_stage->SetCanvasDirty();

		m_last_right = pos;
	}

	return false;
}

bool CreateStripOP::OnMouseRightUp(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseRightUp(x, y))
		return true;

	m_last_right.setInvalid();

	return false;
}

bool CreateStripOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseDrag(x, y))
		return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);

	// move background
	if (m_last_right.isValid())
	{
		d2d::Vector offset = pos - m_last_right;
		StagePanel* stage = static_cast<StagePanel*>(m_stage);
		stage->TranslateBackground(offset);
		m_last_right = pos;
	}

	if (!m_selected) {
		return false;
	}

	if (EditShape* shape = static_cast<EditShape*>(m_stage->GetShape()))
	{
		shape->MoveNode(m_selected, pos);
		m_stage->SetCanvasDirty();
	}

	return false;
}

bool CreateStripOP::OnDraw() const
{
 	if (const d2d::Image* image = m_stage->GetSymbol()->getImage())
  	{
 		d2d::Matrix mt;
 		image->Draw(mt, image->GetClippedRegion());
  	}

	if (Shape* shape = m_stage->GetShape())
	{
		shape->DrawInfoUV();
	}

	d2d::ZoomViewOP::OnDraw();

	return false;
}

}