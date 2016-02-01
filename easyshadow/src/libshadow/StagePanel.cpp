#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "Shadow.h"

#include <easyshape.h>

namespace eshadow
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::MultiShapesImpl()
	, m_loop(NULL)
{
	SetCanvas(new StageCanvas(this));

	m_symbol = new Symbol;

	RegistSubject(ee::InsertShapeSJ::Instance());
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, ee::Sprite* edited, 
					   const ee::MultiSpritesImpl* bg_sprites)
	: ee::EditPanel(parent, frame)
	, ee::MultiShapesImpl()
	, m_loop(NULL)
{
	SetCanvas(new StageCanvas(this, glctx, edited, bg_sprites));

	m_symbol = (Symbol*)(&edited->GetSymbol());
	if (m_symbol) {
		m_symbol->Retain();
	}
	LoadFromShadow();

	RegistSubject(ee::InsertShapeSJ::Instance());
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
	ee::EditPanel::Refresh(eraseBackground, rect);

	if (m_loop) {
		eshape::PolygonShape* poly = static_cast<eshape::PolygonShape*>(m_loop);
		m_symbol->GetShadow()->BuildInnerLine(poly->GetVertices());
	}	
}

void StagePanel::TraverseShapes(ee::Visitor& visitor, ee::DataTraverseType type) const
{
	if (m_loop) {
		bool next;
		visitor.Visit(m_loop, next);
	}
}

void StagePanel::LoadFromShadow()
{
	const Shadow* shadow = m_symbol->GetShadow();
	const std::vector<ee::Vector>& loop = shadow->GetInnerLoop();
	if (m_loop) {
		m_loop->Release();
	}
	m_loop = new eshape::PolygonShape(loop);
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::MultiShapesImpl::OnNotify(sj_id, ud);

	if (sj_id == ee::MSG_INSERT_SHAPE) {
		InsertShape((ee::Shape*)ud);
	}
}

void StagePanel::InsertShape(ee::Shape* shape)
{
	if (eshape::get_shape_type(shape->GetShapeDesc()) != eshape::ST_POLYGON) {
		return;
	}

	m_loop = shape;
	m_loop->Retain();

	eshape::PolygonShape* poly = static_cast<eshape::PolygonShape*>(shape);
	m_symbol->GetShadow()->BuildInnerLine(poly->GetVertices());

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}