#include "StagePanel.h"
#include "StageCanvas.h"
#include "Sprite.h"

namespace emesh
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiShapesImpl(parent)
	, m_background(NULL)
{
	m_sprite = new Sprite();

	init(NULL);
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiShapesImpl(parent)
	, m_background(NULL)
{
	m_sprite = new Sprite();

	init(library);
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library, Sprite* sprite)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiShapesImpl(parent)
	, m_background(NULL)
{
	sprite->retain();
	m_sprite = sprite;

	init(library);
}

StagePanel::~StagePanel()
{
	m_sprite->release();

	if (m_background) {
		m_background->release();
	}
}

void StagePanel::clear()
{
	m_sprite->release();
	m_sprite = new Sprite;
}

void StagePanel::traverseShapes(d2d::IVisitor& visitor, 
								d2d::TraverseType type/* = d2d::e_allExisting*/) const
{
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
	{
		bool hasNext;
		visitor.visit(m_shapes[i], hasNext);
		if (!hasNext) break;
	}
}

void StagePanel::removeShape(d2d::IShape* shape)
{
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
	{
		if (m_shapes[i] == shape)
		{
			m_shapes.erase(m_shapes.begin() + i);
			shape->release();
			break;
		}
	}
}

void StagePanel::insertShape(d2d::IShape* shape)
{
	m_shapes.push_back(shape);
}

void StagePanel::clearShapes()
{
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
		m_shapes[i]->release();
	m_shapes.clear();
}

Shape* StagePanel::getShape()
{
	return m_sprite->getShape();
}

void StagePanel::init(d2d::LibraryPanel* library)
{
	m_editOP = new d2d::ZoomViewOP(this, true);
	m_canvas = new StageCanvas(this);

	if (library) {
		d2d::Frame* frame = static_cast<d2d::Frame*>(m_frame);
		SetDropTarget(new StageDropTarget(frame, this, library));
	}
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(d2d::Frame* frame, StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::StageDropTarget(frame, stage, NULL, library)
	, m_stage(stage)
{
}

bool StagePanel::StageDropTarget::
OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos)
{
	if (d2d::ImageSymbol* image = dynamic_cast<d2d::ImageSymbol*>(symbol))
	{
		Sprite* sprite = new Sprite(new Symbol(image->getImage()));
		m_stage->m_sprite->release();
		m_stage->m_sprite = sprite;
		m_stage->Refresh();

		m_stage->getCanvas()->resetViewport();

		return true;
	}

	return false;
}

}