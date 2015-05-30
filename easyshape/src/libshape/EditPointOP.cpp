#include "EditPointOP.h"
#include "PointShape.h"
#include "ShapeType.h"

namespace libshape
{

EditPointOP::EditPointOP(d2d::EditPanel* editpanel,
						 d2d::MultiShapesImpl* shapes_impl,
						 d2d::OneFloatValue* node_capture)
	: d2d::ZoomViewOP(editpanel, true)
	, m_shapes_impl(shapes_impl)
	, m_node_capture(node_capture)
{
	clear();
}

bool EditPointOP::onKeyDown(int keyCode)
{
	if (d2d::ZoomViewOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapes_impl->removeShapeSelection();
		m_captured.clear();
		m_stage->Refresh();
	}

	return false;
}

bool EditPointOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_pos = m_stage->transPosScreenToProject(x, y);

	m_shapes_impl->getShapeSelection()->Clear();
	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance != 0)
	{	
		NodeCapture capture(m_shapes_impl, tolerance);
		capture.captureEditable(m_pos, m_captured);
		if (m_captured.shape &&
			get_shape_type(m_captured.shape->GetShapeDesc()) == ST_POINT) {
			m_shapes_impl->getShapeSelection()->Add(m_captured.shape);
		}
	}
	else
	{
		m_captured.clear();
	}

	return false;
}

bool EditPointOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (!m_pos.isValid()) {
		return false;
	}

	m_pos = m_stage->transPosScreenToProject(x, y);
	if (!m_captured.shape) {
		PointShape* point = new PointShape(m_pos);
		m_shapes_impl->getShapeSelection()->Add(point);
		m_shapes_impl->insertShape(point);
	}

	clear();

	m_stage->Refresh();

	return false;
}

bool EditPointOP::onMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseRightDown(x, y)) return true;

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance == 0) {
		return false;
	}

	d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
	NodeCapture capture(m_shapes_impl, tolerance);
	capture.captureEditable(pos, m_captured);
	if (m_captured.shape) {
		m_shapes_impl->removeShape(m_captured.shape);
		m_shapes_impl->getShapeSelection()->Clear();
		m_captured.clear();
		m_stage->Refresh();
	}

	return false;
}

bool EditPointOP::onMouseMove(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseMove(x, y)) return true;

	int tolerance = m_node_capture ? m_node_capture->GetValue() : 0;
	if (tolerance == 0) {
		return false;
	}

	d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
	NodeCapture capture(m_shapes_impl, tolerance);
	d2d::IShape* old = m_captured.shape;
	capture.captureEditable(pos, m_captured);
	if (old && !m_captured.shape || !old && m_captured.shape)
		m_stage->Refresh();

	return false;
}

bool EditPointOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y)) return true;

	m_pos = m_stage->transPosScreenToProject(x, y);
	if (m_captured.shape && 
		get_shape_type(m_captured.shape->GetShapeDesc()) == ST_POINT) {		
		PointShape* point = static_cast<PointShape*>(m_captured.shape);
		point->SetPos(m_pos);
		m_stage->Refresh();
	}

	return false;
}

bool EditPointOP::onDraw() const
{
	if (d2d::ZoomViewOP::onDraw()) return true;

	if (m_pos.isValid()) {
		d2d::PrimitiveDraw::drawCircle(m_pos, m_node_capture->GetValue(), true, 2, d2d::LIGHT_RED);
	}

	return false;
}

bool EditPointOP::clear()
{
	if (d2d::ZoomViewOP::clear()) return true;

	m_pos.setInvalid();

	return false;
}

}