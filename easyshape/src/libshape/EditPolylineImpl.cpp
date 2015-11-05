#include "EditPolylineImpl.h"
#include "DrawPolylineOP.h"
#include "ChainShape.h"
#include "PolygonShape.h"

#include "PolygonPropertySetting.h"
#include "ChainPropertySetting.h"

#include "IOperaterBase.h"

namespace libshape
{

EditPolylineImpl::EditPolylineImpl(wxWindow* wnd, d2d::EditPanelImpl* stage,
								   d2d::MultiShapesImpl* shapesImpl,
								   d2d::PropertySettingPanel* propertyPanel, 
								   d2d::OneFloatValue* node_capture, 
								   DrawPolylineOP* draw_op,
								   d2d::AbstractEditOP* select_op,
								   IOperaterBase* base_op)
{
	m_stage = stage;

	m_shapesImpl = shapesImpl;

	m_propertyPanel = propertyPanel;

	m_node_capture = node_capture;

	m_selectOP = select_op;
	m_lastLeftDownPos.setInvalid();
	m_bSelectOpen = false;

	m_draw_op = draw_op;

	m_base_op = base_op;

	m_dirty = false;
}

bool EditPolylineImpl::OnKeyDown(int keyCode)
{
	if (keyCode == WXK_DELETE)
	{
		m_capturedEditable.clear();
		m_captureSelectable.clear();
	}
	return m_selectOP->OnKeyDown(keyCode);
}

bool EditPolylineImpl::OnMouseLeftDown(int x, int y)
{
	if (m_draw_op->ShouldFixPos())
	{
		return m_base_op->OnMouseLeftDownBase(x, y);
	}
	else
	{
		int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
		if (tolerance != 0)
		{	
			NodeCapture capture(m_shapesImpl, tolerance);
			d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
			capture.captureEditable(pos, m_capturedEditable);
			if (!m_capturedEditable.shape)
				capture.captureSelectable(pos, m_captureSelectable);
		}

		if (m_capturedEditable.shape)
		{
			// 			d2d::Vector screen = m_stage->transPosProjectToScreen(m_capturedEditable.pos);
			// 			bNotDeliver = m_draw_op->OnMouseLeftDown(screen.x, screen.y);

			if (m_capturedEditable.pos.isValid())
				m_draw_op->m_polyline.push_back(m_capturedEditable.pos);

			checkActiveShape(m_capturedEditable);
		}
		else if (m_captureSelectable.shape)
		{
			if (m_captureSelectable.pos.isValid())
				m_draw_op->m_polyline.push_back(m_captureSelectable.pos);

			checkActiveShape(m_captureSelectable);
		}
		else
		{
			if (m_draw_op->m_polyline.empty())
				m_lastLeftDownPos = d2d::Vector(x, y);

			if (tolerance != 0 && m_draw_op->m_polyline.empty())
			{
				d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
				InterruptChainVisitor interrupt(pos, tolerance);
				m_shapesImpl->TraverseShapes(interrupt, d2d::DT_EDITABLE);
				if (interrupt.getInterruptedChain())
				{
					m_capturedEditable.shape = interrupt.getInterruptedChain();
					m_capturedEditable.pos = pos;
					m_stage->SetCanvasDirty();

					checkActiveShape(m_capturedEditable);
				}
				else
				{
					int sz = m_shapesImpl->GetShapeSelection()->Size();

					d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
					d2d::IShape* shape = m_shapesImpl->QueryShapeByPos(pos);

					return m_base_op->OnMouseLeftDownBase(x, y);
				}
			}
			else
			{
				return m_base_op->OnMouseLeftDownBase(x, y);
			}
		}
	}

	return false;
}

bool EditPolylineImpl::OnMouseLeftUp(int x, int y)
{
	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
		NearestNodeVisitor nearest(pos, tolerance);
		m_shapesImpl->TraverseShapes(nearest, d2d::DT_VISIBLE);
		float dis = d2d::Math::getDistance(nearest.getNearestNode(), pos);
		if (dis < tolerance)
		{
			if (m_capturedEditable.shape)
			{
				ChainShape* chain = dynamic_cast<ChainShape*>(m_capturedEditable.shape);
				chain->Change(m_capturedEditable.pos, nearest.getNearestNode());
				chain->refresh();
				m_dirty = true;
				m_capturedEditable.pos = nearest.getNearestNode();
				m_stage->SetCanvasDirty();
			}
		}

		if (m_capturedEditable.shape)
		{
			if (m_propertyPanel) {
				m_propertyPanel->EnablePropertyGrid(true);
			}
			checkActiveShape(m_capturedEditable);
		}
	}

	if (m_bSelectOpen)
	{
		m_selectOP->OnMouseLeftUp(x, y);
		m_bSelectOpen = false;
		m_lastLeftDownPos.setInvalid();
	}


	return false;
}

bool EditPolylineImpl::OnMouseRightDown(int x, int y)
{
	if (m_draw_op->m_polyline.empty())
	{
		int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
		if (tolerance != 0)
		{
			NodeCapture capture(m_shapesImpl, tolerance);
			capture.captureEditable(m_stage->TransPosScrToProj(x, y), m_capturedEditable);
			if (m_capturedEditable.shape)
			{
				if (m_capturedEditable.pos.isValid())
				{
					ChainShape* chain = dynamic_cast<ChainShape*>(m_capturedEditable.shape);
					chain->Remove(m_capturedEditable.pos);
					chain->refresh();
					m_dirty = true;
				}
				else
				{
					d2d::RemoveShapeSJ::Instance()->Remove(m_capturedEditable.shape);
					m_shapesImpl->GetShapeSelection()->Clear();
					m_capturedEditable.clear();
					m_captureSelectable.clear();
					m_dirty = true;
					d2d::SelectShapeSJ::Instance()->Select(NULL);
				}
				m_capturedEditable.shape = NULL;
				m_stage->SetCanvasDirty();
			}
		}

		return false;
	}
	else
	{
		return m_base_op->OnMouseRightDownBase(x, y);
	}
}

bool EditPolylineImpl::OnMouseMove(int x, int y)
{
	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{
		d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
		NodeCapture capture(m_shapesImpl, tolerance);
		{
			d2d::IShape* old = m_capturedEditable.shape;
			capture.captureEditable(pos, m_capturedEditable);
			if (old && !m_capturedEditable.shape || !old && m_capturedEditable.shape) {
				m_stage->SetCanvasDirty();
			}
		}
		{
			d2d::IShape* old = m_captureSelectable.shape;
			capture.captureSelectable(pos, m_captureSelectable);
			if (old && !m_captureSelectable.shape || !old && m_captureSelectable.shape) {
				m_stage->SetCanvasDirty();
			}
		}
	}

	return false;
}

bool EditPolylineImpl::OnMouseDrag(int x, int y)
{
	if (m_bSelectOpen)
		m_selectOP->OnMouseDrag(x, y);
	else if (m_capturedEditable.shape)
	{
		if (m_draw_op->m_polyline.size() > 1)
		{
			m_capturedEditable.shape = NULL;
			return false;
		}

		if (m_draw_op->m_polyline.size() == 1)
			m_draw_op->m_polyline.clear();

		d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
		if (ChainShape* chain = dynamic_cast<ChainShape*>(m_capturedEditable.shape))
		{
			if (m_capturedEditable.pos.isValid())
			{
				chain->Change(m_capturedEditable.pos, pos);
				m_capturedEditable.pos = pos;
			}
			else
			{
				d2d::Vector old;
				old.x = chain->getRect().xCenter();
				old.y = chain->getRect().yCenter();
				chain->Translate(pos - old);
			}

			chain->refresh();
			m_dirty = true;
			m_stage->SetCanvasDirty();

			if (m_propertyPanel) {
				m_propertyPanel->EnablePropertyGrid(false);
			}
		}
	}
	else if (m_lastLeftDownPos.isValid() 
		&& d2d::Math::getDistance(m_lastLeftDownPos, d2d::Vector(x, y)) < DRAG_SELECT_TOL)
		//		&& (m_lastLeftDownPos.x != x || m_lastLeftDownPos.y != y))
	{
		if (m_draw_op->m_polyline.size() == 1)
			m_draw_op->m_polyline.clear();
		m_bSelectOpen = true;
		m_selectOP->OnMouseLeftDown(x, y);
	}

	return false;
}

void EditPolylineImpl::OnDraw() const
{
	m_selectOP->OnDraw();
	if (m_node_capture)
	{
		if (m_capturedEditable.shape) {
			drawCaptured(m_capturedEditable);
		} else if (m_captureSelectable.shape) {
			drawCaptured(m_captureSelectable);
		}
	}
}

void EditPolylineImpl::Clear()
{
	m_selectOP->Clear();
	m_capturedEditable.shape = NULL;
	m_captureSelectable.shape = NULL;
}

void EditPolylineImpl::drawCaptured(const NodeAddr& captured) const
{
	if (ChainShape* chain = dynamic_cast<ChainShape*>(captured.shape))
	{
		if (captured.pos.isValid()) {
			d2d::PrimitiveDraw::drawCircle(captured.pos, m_node_capture->GetValue(), true, 2, d2d::Colorf(1.0f, 0.4f, 0.4f));
		}

		d2d::Vector center;
		center.x = chain->getRect().xCenter();
		center.y = chain->getRect().yCenter();
		d2d::PrimitiveDraw::drawCircle(center, m_node_capture->GetValue(), true, 2, d2d::Colorf(0.4f, 1.0f, 0.4f));
	}
}

void EditPolylineImpl::checkActiveShape(const NodeAddr& captured)
{
	d2d::SelectShapeSJ::Instance()->Select(captured.shape);

	d2d::ShapeSelection* selection = m_shapesImpl->GetShapeSelection();
	selection->Clear();
	selection->Add(captured.shape);
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylineImpl::InterruptChainVisitor
//////////////////////////////////////////////////////////////////////////

EditPolylineImpl::InterruptChainVisitor::
InterruptChainVisitor(const d2d::Vector& pos, int tol)
: m_pos(pos)
, m_tol(tol)
, m_chain(NULL)
{
}

void EditPolylineImpl::InterruptChainVisitor::
Visit(d2d::Object* object, bool& bFetchNext) 
{
	d2d::Rect rect(m_pos, m_tol, m_tol);

	ChainShape* chain = static_cast<ChainShape*>(object);
	if (!chain->isIntersect(rect)) 
	{
		bFetchNext = true;
		return;
	}

	size_t iPos;
	const std::vector<d2d::Vector>& vertices = chain->GetVertices();
	float dis = d2d::Math::getDisPointToPolyline(m_pos, vertices, &iPos);
	if (dis < m_tol)
	{
		chain->Add(iPos + 1, m_pos);
		m_chain = chain;
		bFetchNext = false;
		return;
	}
	else if (chain->IsClosed() && vertices.size() > 1)
	{
		float dis = d2d::Math::getDisPointToSegment(m_pos, vertices.front(), vertices.back());
		if (dis < m_tol)
		{
			chain->Add(vertices.size(), m_pos);
			m_chain = chain;
			bFetchNext = false;
			return;
		}
	}

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylineImpl::NearestNodeVisitor
//////////////////////////////////////////////////////////////////////////

EditPolylineImpl::NearestNodeVisitor::
NearestNodeVisitor(const d2d::Vector& pos, int tol)
: m_pos(pos)
, m_tol(tol)
{
	m_dis = FLT_MAX;
	m_nearest.setInvalid();
}

void EditPolylineImpl::NearestNodeVisitor::
Visit(d2d::Object* object, bool& bFetchNext)
{
	ChainShape* chain = dynamic_cast<ChainShape*>(object);
	if (!chain) 
	{
		bFetchNext = true;
		return;
	}

	d2d::Rect rect(m_pos, m_tol, m_tol);

	if (!chain->isIntersect(rect)) 
	{
		bFetchNext = true;
		return;
	}

	const std::vector<d2d::Vector>& vertices = chain->GetVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		float dis = d2d::Math::getDistance(m_pos, vertices[i]);
		if (dis < m_dis && dis != 0)
		{
			m_dis = dis;
			m_nearest = vertices[i];
		}
	}

	bFetchNext = true;
}

}