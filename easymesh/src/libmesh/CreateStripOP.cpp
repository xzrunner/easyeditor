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

bool CreateStripOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y))
		return true;

	if (EditShape* shape = static_cast<EditShape*>(m_stage->GetShape()))
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		m_selected = shape->FindNode(pos);
	}

	return false;
}

bool CreateStripOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftUp(x, y))
		return true;

	if (m_selected) {
		return false;
	}

	if (EditShape* shape = static_cast<EditShape*>(m_stage->GetShape()))
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		shape->InsertNode(pos);
		m_editPanel->Refresh();
	}

	return false;
}

bool CreateStripOP::onMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseRightDown(x, y))
		return true;

	if (EditShape* shape = static_cast<EditShape*>(m_stage->GetShape()))
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		shape->RemoveNode(pos);
		m_editPanel->Refresh();

		m_last_right = pos;
	}

	return false;
}

bool CreateStripOP::onMouseRightUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseRightUp(x, y))
		return true;

	m_last_right.setInvalid();

	return false;
}

bool CreateStripOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y))
		return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);

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
		m_stage->Refresh();
	}

	return false;
}

bool CreateStripOP::onDraw() const
{
 	if (const d2d::Image* image = m_stage->GetSymbol()->getImage())
  	{
 		d2d::Matrix mt;
 		image->draw(mt, image->getRegion());
  	}

	if (Shape* shape = m_stage->GetShape())
	{
		shape->DrawInfoUV();
	}

	d2d::ZoomViewOP::onDraw();

	return false;
}

}