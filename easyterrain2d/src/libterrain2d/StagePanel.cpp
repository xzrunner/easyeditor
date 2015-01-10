#include "StagePanel.h"
#include "StageCanvas.h"

namespace eterrain2d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, d2d::ShapesPanelImpl(this)
{
	m_editOP = new d2d::ZoomViewOP(this, true);
	m_canvas = new StageCanvas(this);

// 	d2d::Frame* _frame = static_cast<d2d::Frame*>(m_frame);
// 	SetDropTarget(new StageDropTarget(_frame, this, library));
}

StagePanel::~StagePanel()
{
	
}

void StagePanel::clear()
{

}

}