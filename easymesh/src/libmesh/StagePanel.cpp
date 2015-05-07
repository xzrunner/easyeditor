#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "EditShape.h"

#include <easyshape.h>

namespace emesh
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiShapesImpl(parent)
	, m_background(NULL)
{
	m_symbol = new Symbol;
	init(NULL);
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiShapesImpl(parent)
	, m_background(NULL)
{
	m_symbol = new Symbol;
	init(library);
}

// StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
// 					   d2d::LibraryPanel* library, Sprite* sprite)
// 	: d2d::EditPanel(parent, frame)
// 	, d2d::MultiShapesImpl(parent)
// 	, m_background(NULL)
// {
// 	sprite->Retain();
// 	m_sprite = sprite;
// 
// 	init(library);
// }

StagePanel::~StagePanel()
{
	m_symbol->Release();
	if (m_background) {
		m_background->Release();
	}
}

void StagePanel::clear()
{
	m_symbol->Release();
	m_symbol = new Symbol;
}

void StagePanel::traverseShapes(d2d::IVisitor& visitor, 
								d2d::DataTraverseType type/* = d2d::DT_ALL*/) const
{
	Shape* shape = m_symbol->getShape();
	if (shape) {
		static_cast<EditShape*>(shape)->TraverseShape(visitor);
	}
}

void StagePanel::removeShape(d2d::IShape* shape)
{
	Shape* _shape = m_symbol->getShape();
	if (_shape) {
		static_cast<EditShape*>(_shape)->RemoveShape(shape);
	}
}

void StagePanel::insertShape(d2d::IShape* shape)
{
	Shape* _shape = m_symbol->getShape();
	if (_shape) {
		static_cast<EditShape*>(_shape)->InsertShape(shape);
		_shape->Refresh();
	}
}

void StagePanel::clearShapes()
{
	Shape* shape = m_symbol->getShape();
	if (shape) {
		static_cast<EditShape*>(shape)->ClearShape();
	}
}

void StagePanel::SetSymbol(Symbol* symbol)
{
	if (m_symbol != symbol) {
		m_symbol->Release();
		m_symbol = symbol;
		m_symbol->Retain();
	}
}

const Symbol* StagePanel::GetSymbol() const
{
	return m_symbol;
}

Shape* StagePanel::GetShape()
{
	return m_symbol->getShape();
}

void StagePanel::LoadFromSymbol(const d2d::ISymbol* symbol)
{
}

void StagePanel::UpdateSymbol()
{
	if (Shape* shape = m_symbol->getShape()) {
		std::vector<const libshape::ChainShape*> polylines;
		traverseShapes(d2d::FetchAllVisitor<const libshape::ChainShape>(polylines));
		shape->Refresh();
	}
}

void StagePanel::CreateShape()
{
	if (m_symbol) {
		m_symbol->CreateShape();
	}
}

void StagePanel::init(d2d::LibraryPanel* library)
{
	m_editOP = new d2d::ZoomViewOP(this, true);
	m_canvas = new StageCanvas(this);

	if (library) {
		SetDropTarget(new StageDropTarget(this, library));
	}
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::StageDropTarget(stage, NULL, library)
	, m_stage(stage)
{
}

bool StagePanel::StageDropTarget::
OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos)
{
	if (d2d::ImageSymbol* image = dynamic_cast<d2d::ImageSymbol*>(symbol))
	{
		Symbol* symbol = new Symbol(image->getImage());
		m_stage->m_symbol->Release();
		m_stage->m_symbol = symbol;
		m_stage->Refresh();

		m_stage->getCanvas()->resetViewport();

		return true;
	}

	return false;
}

}