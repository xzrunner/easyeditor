#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "Shadow.h"

#include <ee/shape_msg.h>
#include <ee/subject_id.h>
#include <ee/panel_msg.h>

#include <easyshape.h>

namespace eshadow
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::MultiShapesImpl()
	, m_loop(NULL)
{
	SetCanvas(std::make_shared<StageCanvas>(this));

	m_sym = new Symbol;

	RegistSubject(ee::InsertShapeSJ::Instance());
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, const ee::SprPtr& edited, 
					   const ee::MultiSpritesImpl* bg_sprites)
	: ee::EditPanel(parent, frame)
	, ee::MultiShapesImpl()
	, m_loop(NULL)
{
	SetCanvas(std::make_shared<StageCanvas>(this, glctx, edited, bg_sprites));

	m_sym = dynamic_cast<Symbol*>(edited->GetSymbol());
	if (m_sym) {
		m_sym->AddReference();
	}
	LoadFromShadow();

	RegistSubject(ee::InsertShapeSJ::Instance());
}

StagePanel::~StagePanel()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
	if (m_loop) {
		m_loop->RemoveReference();
	}
}

void StagePanel::Refresh(bool eraseBackground, const wxRect* rect)
{
	ee::EditPanel::Refresh(eraseBackground, rect);

	if (m_loop) {
		eshape::PolygonShape* poly = static_cast<eshape::PolygonShape*>(m_loop);
		m_sym->GetShadow()->BuildInnerLine(poly->GetVertices());
	}	
}

void StagePanel::TraverseShapes(ee::RefVisitor<ee::Shape>& visitor, ee::DataTraverseType type) const
{
	if (m_loop) {
		bool next;
		visitor.Visit(m_loop, next);
	}
}

void StagePanel::LoadFromShadow()
{
	const Shadow* shadow = m_sym->GetShadow();
	const std::vector<sm::vec2>& loop = shadow->GetInnerLoop();
	if (m_loop) {
		m_loop->RemoveReference();
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
	m_loop->AddReference();

	eshape::PolygonShape* poly = static_cast<eshape::PolygonShape*>(shape);
	m_sym->GetShadow()->BuildInnerLine(poly->GetVertices());

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}