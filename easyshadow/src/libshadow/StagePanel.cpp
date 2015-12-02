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
	, d2d::MultiShapesImpl()
	, m_loop(NULL)
{
	SetCanvas(new StageCanvas(this));

	m_symbol = new Symbol;

	RegistSubject(d2d::InsertShapeSJ::Instance());
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiShapesImpl()
	, m_loop(NULL)
{
	SetCanvas(new StageCanvas(this, edited, bg_sprites));

	m_symbol = (Symbol*)(&edited->GetSymbol());
	if (m_symbol) {
		m_symbol->Retain();
	}
	LoadFromShadow();

	RegistSubject(d2d::InsertShapeSJ::Instance());
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

void StagePanel::TraverseShapes(d2d::IVisitor& visitor, d2d::DataTraverseType type) const
{
	if (m_loop) {
		bool hasNext;
		visitor.Visit(m_loop, hasNext);
	}
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

void StagePanel::OnNotify(int sj_id, void* ud)
{
	d2d::MultiShapesImpl::OnNotify(sj_id, ud);

	if (sj_id == d2d::MSG_INSERT_SHAPE) {
		InsertShape((d2d::IShape*)ud);
	}
}

void StagePanel::InsertShape(d2d::IShape* shape)
{
	if (libshape::get_shape_type(shape->GetShapeDesc()) != libshape::ST_POLYGON) {
		return;
	}

	m_loop = shape;
	m_loop->Retain();

	libshape::PolygonShape* poly = static_cast<libshape::PolygonShape*>(shape);
	m_symbol->GetShadow()->BuildInnerLine(poly->GetVertices());

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

}