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
{
	m_canvas = new StageCanvas(this);

	m_symbol = new Symbol;
}

StagePanel::~StagePanel()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

void StagePanel::clear()
{
}

void StagePanel::traverseShapes(d2d::IVisitor& visitor, d2d::DataTraverseType type) const
{
}

void StagePanel::removeShape(d2d::IShape* shape)
{	
}

void StagePanel::insertShape(d2d::IShape* shape)
{
	if (libshape::get_shape_type(shape->GetShapeDesc()) != libshape::ST_POLYGON) {
		return;
	}

	libshape::PolygonShape* poly = static_cast<libshape::PolygonShape*>(shape);
	m_symbol->GetShadow()->BuildInnerLine(poly->GetVertices());
}

void StagePanel::clearShapes()
{
}

}