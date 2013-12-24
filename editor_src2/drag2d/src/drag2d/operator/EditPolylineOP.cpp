#ifndef D2D_EDIT_POLYLINE_OP_CPP
#define D2D_EDIT_POLYLINE_OP_CPP

#include "EditPolylineOP.h"

#include "common/Math.h"
#include "dataset/ChainShape.h"
#include "dataset/PolygonShape.h"
#include "render/PrimitiveDraw.h"
#include "view/PropertySettingPanel.h"
#include "view/ChainPropertySetting.h"
#include "view/PolygonPropertySetting.h"

template <typename TBase, typename TSelected>
d2d::EditPolylineOP<TBase, TSelected>::
EditPolylineOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			   PropertySettingPanel* propertyPanel,
			   NodeCaptureCMPT<EditPolylineOP>* cmpt)
	: TBase(editPanel, shapesImpl)
{
	m_shapesImpl = shapesImpl;

	m_propertyPanel = propertyPanel;

	m_cmpt = cmpt;

	m_selectOP = new TSelected(editPanel, shapesImpl, propertyPanel, cmpt);
	m_lastLeftDownPos.setInvalid();
	m_bSelectOpen = false;
}

template <typename TBase, typename TSelected>
d2d::EditPolylineOP<TBase, TSelected>::
~EditPolylineOP() 
{
	delete m_selectOP;
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onKeyDown(int keyCode) 
{
	if (keyCode == WXK_DELETE)
	{
		m_capturedEditable.clear();
		m_captureSelectable.clear();
	}
	return m_selectOP->onKeyDown(keyCode);
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onMouseLeftDown(int x, int y) 
{
	bool bNotDeliver = false;

	if (shouldFixMousePos())
		bNotDeliver = TBase::onMouseLeftDown(x, y);
	else
	{
		int tolerance;
		if (m_cmpt)
			tolerance = m_cmpt->getNodeCaptureDistance();
		else
			tolerance = 0;
		if (tolerance != 0)
		{	
			NodeCapture capture(m_shapesImpl, tolerance);
			Vector pos = m_editPanel->transPosScreenToProject(x, y);
			capture.captureEditable(pos, m_capturedEditable);
			if (!m_capturedEditable.shape)
				capture.captureSelectable(pos, m_captureSelectable);
		}

		if (m_capturedEditable.shape)
		{
// 			Vector screen = m_editPanel->transPosProjectToScreen(m_capturedEditable.pos);
// 			bNotDeliver = TBase::onMouseLeftDown(screen.x, screen.y);

			if (m_capturedEditable.pos.isValid())
				m_polyline.push_back(m_capturedEditable.pos);

			checkActiveShape(m_capturedEditable);
		}
		else if (m_captureSelectable.shape)
		{
			if (m_captureSelectable.pos.isValid())
				m_polyline.push_back(m_captureSelectable.pos);

			checkActiveShape(m_captureSelectable);
		}
		else
		{
			if (m_polyline.empty())
				m_lastLeftDownPos = Vector(x, y);

			if (tolerance != 0 && m_polyline.empty())
			{
				Vector pos = m_editPanel->transPosScreenToProject(x, y);
				InterruptChainVisitor interrupt(pos, tolerance);
				m_shapesImpl->traverseShapes(interrupt, e_editable);
				if (interrupt.getInterruptedChain())
				{
					m_capturedEditable.shape = interrupt.getInterruptedChain();
					m_capturedEditable.pos = pos;
					m_editPanel->Refresh();

					checkActiveShape(m_capturedEditable);
				}
				else
					bNotDeliver = TBase::onMouseLeftDown(x, y);
			}
			else
				bNotDeliver = TBase::onMouseLeftDown(x, y);
		}
	}

	return bNotDeliver;
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onMouseLeftUp(int x, int y) 
{
	if (TBase::onMouseLeftUp(x, y)) return true;

	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
	if (tolerance != 0)
	{	
		Vector pos = m_editPanel->transPosScreenToProject(x, y);
		NearestNodeVisitor nearest(pos, tolerance);
		m_shapesImpl->traverseShapes(nearest, e_visible);
		float dis = Math::getDistance(nearest.getNearestNode(), pos);
		if (dis < tolerance)
		{
			if (m_capturedEditable.shape)
			{
				ChainShape* chain = dynamic_cast<ChainShape*>(m_capturedEditable.shape);
				chain->changeVertices(m_capturedEditable.pos, nearest.getNearestNode());
				chain->refresh();
				m_capturedEditable.pos = nearest.getNearestNode();
				m_editPanel->Refresh();
			}
		}

		if (m_capturedEditable.shape)
		{
			m_propertyPanel->enablePropertyGrid(true);
			checkActiveShape(m_capturedEditable);
		}
	}

	if (m_bSelectOpen)
	{
		m_selectOP->onMouseLeftUp(x, y);
		m_bSelectOpen = false;
		m_lastLeftDownPos.setInvalid();
	}

	m_editPanel->Refresh();

	return false;
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onMouseRightDown(int x, int y) 
{
	if (m_polyline.empty())
	{
		int tolerance;
		if (m_cmpt)
			tolerance = m_cmpt->getNodeCaptureDistance();
		else
			tolerance = 0;
		if (tolerance != 0)
		{
			NodeCapture capture(m_shapesImpl, tolerance);
			capture.captureEditable(m_editPanel->transPosScreenToProject(x, y), m_capturedEditable);
			if (m_capturedEditable.shape)
			{
				if (m_capturedEditable.pos.isValid())
				{
					ChainShape* chain = dynamic_cast<ChainShape*>(m_capturedEditable.shape);
					chain->removeVertices(m_capturedEditable.pos);
					chain->refresh();
				}
				else
				{
					m_shapesImpl->removeShape(m_capturedEditable.shape);
					m_shapesImpl->getShapeSelection()->clear();
					m_capturedEditable.clear();
					m_captureSelectable.clear();
					m_propertyPanel->setPropertySetting(NULL);
				}
				m_capturedEditable.shape = NULL;
				m_editPanel->Refresh();
			}
		}

		return false;
	}
	else
	{
		return TBase::onMouseRightDown(x, y);
	}
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onMouseMove(int x, int y) 
{
	if (TBase::onMouseMove(x, y)) return true;

	int tolerance;
	if (m_cmpt)
		tolerance = m_cmpt->getNodeCaptureDistance();
	else
		tolerance = 0;
	if (tolerance != 0)
	{
		Vector pos = m_editPanel->transPosScreenToProject(x, y);
		NodeCapture capture(m_shapesImpl, tolerance);
		{
			IShape* old = m_capturedEditable.shape;
			capture.captureEditable(pos, m_capturedEditable);
			if (old && !m_capturedEditable.shape || !old && m_capturedEditable.shape)
				m_editPanel->Refresh();
		}
		{
			IShape* old = m_captureSelectable.shape;
			capture.captureSelectable(pos, m_captureSelectable);
			if (old && !m_captureSelectable.shape || !old && m_captureSelectable.shape)
				m_editPanel->Refresh();
		}
	}

	return false;
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onMouseDrag(int x, int y) 
{
	if (TBase::onMouseDrag(x, y)) return true;

	if (m_bSelectOpen)
		m_selectOP->onMouseDrag(x, y);
	else if (m_capturedEditable.shape)
	{
		if (m_polyline.size() > 1)
		{
			m_capturedEditable.shape = NULL;
			return false;
		}

		if (m_polyline.size() == 1)
			m_polyline.clear();

		Vector pos = m_editPanel->transPosScreenToProject(x, y);
		if (ChainShape* chain = dynamic_cast<ChainShape*>(m_capturedEditable.shape))
		{
			if (m_capturedEditable.pos.isValid())
			{
				chain->changeVertices(m_capturedEditable.pos, pos);
				m_capturedEditable.pos = pos;
			}
			else
			{
				Vector old;
				old.x = chain->getRect().xCenter();
				old.y = chain->getRect().yCenter();
				Vector offset = pos - old;

				std::vector<Vector> ctlpos = chain->getVertices();
				for (size_t i = 0, n = ctlpos.size(); i < n; ++i)
					ctlpos[i] += offset;
				chain->setVertices(ctlpos);
			}

			chain->refresh();
			m_editPanel->Refresh();

			m_propertyPanel->enablePropertyGrid(false);
		}
	}
	else if (m_lastLeftDownPos.isValid() 
		&& Math::getDistance(m_lastLeftDownPos, Vector(x, y)) < DRAG_SELECT_TOL)
		//		&& (m_lastLeftDownPos.x != x || m_lastLeftDownPos.y != y))
	{
		if (m_polyline.size() == 1)
			m_polyline.clear();
		m_bSelectOpen = true;
		m_selectOP->onMouseLeftDown(x, y);
	}

	return false;
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
onDraw() const 
{
	if (TBase::onDraw()) return true;

	m_selectOP->onDraw();
	if (m_cmpt)
	{
		if (m_capturedEditable.shape)
			drawCaptured(m_capturedEditable);
		else if (m_captureSelectable.shape)
			drawCaptured(m_captureSelectable);
	}

	return false;
}

template <typename TBase, typename TSelected>
bool d2d::EditPolylineOP<TBase, TSelected>::
clear() 
{
	if (TBase::clear()) return true;

	m_selectOP->clear();
	m_capturedEditable.shape = NULL;
	m_captureSelectable.shape = NULL;

	return false;
}

template <typename TBase, typename TSelected>
void d2d::EditPolylineOP<TBase, TSelected>::
drawCaptured(const NodeAddr& captured) const
{
	if (ChainShape* chain = dynamic_cast<ChainShape*>(captured.shape))
	{
		if (captured.pos.isValid())
			PrimitiveDraw::drawCircle(captured.pos, m_cmpt->getNodeCaptureDistance(), true, 2, Colorf(1.0f, 0.4f, 0.4f));

		Vector center;
		center.x = chain->getRect().xCenter();
		center.y = chain->getRect().yCenter();
		PrimitiveDraw::drawCircle(center, m_cmpt->getNodeCaptureDistance(), true, 2, Colorf(0.4f, 1.0f, 0.4f));
	}
}

template <typename TBase, typename TSelected>
void d2d::EditPolylineOP<TBase, TSelected>::
checkActiveShape(const NodeAddr& captured)
{
	if (PolygonShape* poly = dynamic_cast<PolygonShape*>(captured.shape))
	{
		m_propertyPanel->setPropertySetting(new PolygonPropertySetting(m_editPanel, poly));
		m_shapesImpl->getShapeSelection()->insert(poly);
	}
	else if (ChainShape* chain = dynamic_cast<ChainShape*>(captured.shape))
	{
		m_propertyPanel->setPropertySetting(new ChainPropertySetting(m_editPanel, chain));
		m_shapesImpl->getShapeSelection()->insert(chain);
	}	
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylineOP::InterruptChainVisitor
//////////////////////////////////////////////////////////////////////////

template <typename TBase, typename TSelected>
d2d::EditPolylineOP<TBase, TSelected>::InterruptChainVisitor::
InterruptChainVisitor(const Vector& pos, int tol)
	: m_pos(pos)
	, m_tol(tol)
	, m_chain(NULL)
{
}

template <typename TBase, typename TSelected>
void d2d::EditPolylineOP<TBase, TSelected>::InterruptChainVisitor::
visit(Object* object, bool& bFetchNext) 
{
	Rect rect(m_pos, m_tol, m_tol);

	ChainShape* chain = static_cast<ChainShape*>(object);
	if (!chain->isIntersect(rect)) 
	{
		bFetchNext = true;
		return;
	}

	size_t iPos;
	const std::vector<Vector>& vertices = chain->getVertices();
	float dis = Math::getDisPointToPolyline(m_pos, vertices, &iPos);
	if (dis < m_tol)
	{
		chain->insertVertices(iPos + 1, m_pos);
		m_chain = chain;
		bFetchNext = false;
		return;
	}
	else if (chain->isClosed() && vertices.size() > 1)
	{
		float dis = Math::getDisPointToSegment(m_pos, vertices.front(), vertices.back());
		if (dis < m_tol)
		{
			chain->insertVertices(vertices.size(), m_pos);
			m_chain = chain;
			bFetchNext = false;
			return;
		}
	}

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylineOP::NearestNodeVisitor
//////////////////////////////////////////////////////////////////////////

template <typename TBase, typename TSelected>
d2d::EditPolylineOP<TBase, TSelected>::NearestNodeVisitor::
NearestNodeVisitor(const Vector& pos, int tol)
	: m_pos(pos)
	, m_tol(tol)
{
	m_dis = FLT_MAX;
	m_nearest.setInvalid();
}

template <typename TBase, typename TSelected>
void d2d::EditPolylineOP<TBase, TSelected>::NearestNodeVisitor::
visit(Object* object, bool& bFetchNext)
{
	ChainShape* chain = dynamic_cast<ChainShape*>(object);
	if (!chain) 
	{
		bFetchNext = true;
		return;
	}

	Rect rect(m_pos, m_tol, m_tol);

	if (!chain->isIntersect(rect)) 
	{
		bFetchNext = true;
		return;
	}

	const std::vector<Vector>& vertices = chain->getVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		float dis = Math::getDistance(m_pos, vertices[i]);
		if (dis < m_dis && dis != 0)
		{
			m_dis = dis;
			m_nearest = vertices[i];
		}
	}

	bFetchNext = true;
}

#endif // D2D_EDIT_POLYLINE_OP_CPP