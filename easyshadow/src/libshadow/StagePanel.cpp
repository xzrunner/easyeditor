#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "Shadow.h"

#include <easyshape.h>

namespace eshadow
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiShapesImpl(this)
	, m_loop(NULL)
{
	m_canvas = new StageCanvas(this);

	m_symbol = new Symbol;
}

StagePanel::~StagePanel()
{
	if (m_symbol) {
		m_symbol->Release();
	}
	if (m_loop) {
		m_loop->Release();
	}
}

void StagePanel::Refresh(bool eraseBackground, const wxRect* rect)
{
	d2d::EditPanel::Refresh(eraseBackground, rect);

	if (m_loop) {
		libshape::PolygonShape* poly = static_cast<libshape::PolygonShape*>(m_loop);
		m_symbol->GetShadow()->BuildInnerLine(poly->GetVertices());
	}	
}

void StagePanel::clear()
{
}

void StagePanel::traverseShapes(d2d::IVisitor& visitor, d2d::DataTraverseType type) const
{
	if (m_loop) {
		bool hasNext;
		visitor.visit(m_loop, hasNext);
	}
}

void StagePanel::removeShape(d2d::IShape* shape)
{	
}

void StagePanel::insertShape(d2d::IShape* shape)
{
	if (libshape::get_shape_type(shape->GetShapeDesc()) != libshape::ST_POLYGON) {
		return;
	}

	m_loop = shape;
	m_loop->Retain();

	libshape::PolygonShape* poly = static_cast<libshape::PolygonShape*>(shape);
	m_symbol->GetShadow()->BuildInnerLine(poly->GetVertices());
}

void StagePanel::clearShapes()
{
}

void StagePanel::LoadFromShadow()
{
	const Shadow* shadow = m_symbol->GetShadow();
	const std::vector<d2d::Vector>& loop = shadow->GetInnerLoop();
	if (m_loop) {
		m_loop->Release();
	}
	m_loop = new libshape::PolygonShape(loop);
}

}