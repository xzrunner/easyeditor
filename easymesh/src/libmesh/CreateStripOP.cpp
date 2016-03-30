#include "CreateStripOP.h"
#include "StagePanel.h"
#include "EditShape.h"
#include "Symbol.h"

#include <ee/panel_msg.h>
#include <ee/Matrix.h>
#include <ee/Image.h>
#include <ee/RenderParams.h>

namespace emesh
{

CreateStripOP::CreateStripOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true, false)
	, m_stage(stage)
	, m_selected(NULL)
{
	m_last_right.SetInvalid();
}

bool CreateStripOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y))
		return true;

	if (EditShape* shape = static_cast<EditShape*>(m_stage->GetShape()))
	{
		ee::Vector pos = m_stage->TransPosScrToProj(x, y);
		m_selected = shape->FindNode(pos);
	}

	return false;
}

bool CreateStripOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y))
		return true;

	if (m_selected) {
		return false;
	}

	if (EditShape* shape = static_cast<EditShape*>(m_stage->GetShape()))
	{
		ee::Vector pos = m_stage->TransPosScrToProj(x, y);
		shape->InsertNode(pos);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool CreateStripOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y))
		return true;

	if (EditShape* shape = static_cast<EditShape*>(m_stage->GetShape()))
	{
		ee::Vector pos = m_stage->TransPosScrToProj(x, y);
		shape->RemoveNode(pos);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();

		m_last_right = pos;
	}

	return false;
}

bool CreateStripOP::OnMouseRightUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightUp(x, y))
		return true;

	m_last_right.SetInvalid();

	return false;
}

bool CreateStripOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y))
		return true;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);

	// move background
	if (m_last_right.IsValid())
	{
		ee::Vector offset = pos - m_last_right;
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
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool CreateStripOP::OnDraw() const
{
 	if (const ee::Image* image = m_stage->GetSymbol()->getImage())
  	{
		image->Draw(ee::RenderParams());
  	}

	if (Shape* shape = m_stage->GetShape())
	{
		shape->DrawInfoUV();
	}

	ee::ZoomViewOP::OnDraw();

	return false;
}

}