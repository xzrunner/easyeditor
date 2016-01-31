#include "EditRectOP.h"
#include "RectShape.h"
#include "RectPropertySetting.h"

namespace libshape
{

EditRectOP::EditRectOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
					   ee::MultiShapesImpl* shapesImpl,
					   ee::PropertySettingPanel* propertyPanel,
					   ee::OneFloatValue* node_capture)
	: ee::ZoomViewOP(wnd, stage, true)
	, m_propertyPanel(propertyPanel)
	, m_shapesImpl(shapesImpl)
	, m_node_capture(node_capture)
{
	m_cursor = wxCursor(wxCURSOR_PENCIL);

	Clear();
}

bool EditRectOP::OnKeyDown(int keyCode)
{
	if (ee::ZoomViewOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapesImpl->ClearSelectedShape();
		m_captured.clear();
		ee::SelectShapeSJ::Instance()->Select(NULL);
	}

	return false;
}

bool EditRectOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_firstPress = m_currPos = m_stage->TransPosScrToProj(x, y);

	m_shapesImpl->GetShapeSelection()->Clear();

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapesImpl, tolerance);
		capture.captureEditable(m_firstPress, m_captured);

		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
		{
			m_shapesImpl->GetShapeSelection()->Add(rect);
			ee::SelectShapeSJ::Instance()->Select(rect);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditRectOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (!m_captured.shape)
	{
		if (m_firstPress.IsValid())
		{
			m_currPos = m_stage->TransPosScrToProj(x, y);

			const float dis = ee::Math2D::GetDistance(m_firstPress, m_currPos);
			if (dis > 1)
			{
				RectShape* rect = new RectShape(m_firstPress, m_currPos);
				ee::SelectShapeSJ::Instance()->Select(rect);
				ee::InsertShapeSJ::Instance()->Insert(NULL);
			}
		}
	}
	else
	{
		m_propertyPanel->EnablePropertyGrid(true);
		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape)) {
			ee::SelectShapeSJ::Instance()->Select(rect);
		}
	}

	Clear();


	return false;
}

bool EditRectOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y)) return true;

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{
		m_currPos = m_stage->TransPosScrToProj(x, y);

		NodeCapture capture(m_shapesImpl, tolerance);
		capture.captureEditable(m_currPos, m_captured);
		if (m_captured.shape)
		{
			ee::RemoveShapeSJ::Instance()->Remove(m_captured.shape);
			m_shapesImpl->GetShapeSelection()->Clear();
			m_captured.clear();
			ee::SelectShapeSJ::Instance()->Select(NULL);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditRectOP::OnMouseMove(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseMove(x, y)) return true;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapesImpl, tolerance);
		ee::Shape* old = m_captured.shape;
		capture.captureEditable(pos, m_captured);
		if (old && !m_captured.shape || !old && m_captured.shape) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}

	return false;
}

bool EditRectOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	m_currPos = m_stage->TransPosScrToProj(x, y);

	if (m_captured.shape)
	{
		if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
		{
			ee::Vector center(rect->m_rect.CenterX(), rect->m_rect.CenterY());

			// move
			if (!m_captured.pos.IsValid())
			{
				rect->m_rect.Translate(m_currPos - center);
			}
			// change size
			else 
			{
				if (m_captured.pos.x > center.x)
					rect->m_rect.xmax = m_currPos.x;
				else
					rect->m_rect.xmin = m_currPos.x;
				if (m_captured.pos.y > center.y)
					rect->m_rect.ymax = m_currPos.y;
				else
					rect->m_rect.ymin = m_currPos.y;

				m_captured.pos = m_currPos;
			}

			m_propertyPanel->EnablePropertyGrid(false);

			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}


	return false;
}

bool EditRectOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	if (m_captured.shape)
	{
		if (m_node_capture)
		{
			int tolerance = m_node_capture->GetValue();
			if (RectShape* rect = dynamic_cast<RectShape*>(m_captured.shape))
			{
				ee::Vector pos(rect->m_rect.CenterX(), rect->m_rect.CenterY());
				ee::PrimitiveDraw::DrawCircle(pos, tolerance, true, 2, ee::Colorf(0.4f, 1.0f, 0.4f));
				if (m_captured.pos.IsValid()) {
					ee::PrimitiveDraw::DrawCircle(m_captured.pos, tolerance, true, 2, ee::Colorf(1.0f, 0.4f, 0.4f));
				}
			}
		}
	}
	else
	{
		if (m_firstPress.IsValid() && m_currPos.IsValid())
			ee::PrimitiveDraw::DrawRect(m_firstPress, m_currPos, m_style);
	}

	return false;
}

bool EditRectOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_firstPress.SetInvalid();
	m_currPos.SetInvalid();

	return false;
}

}