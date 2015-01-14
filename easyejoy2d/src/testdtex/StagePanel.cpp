#include "StagePanel.h"
#include "StageCanvas.h"

#include "TestC3.h"

namespace tdtex
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, d2d::ShapesPanelImpl(this)
	, m_test(NULL)
{
	m_editOP = new d2d::ZoomViewOP(this, true);
	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
	delete m_test;
}

void StagePanel::clear()
{
}

void StagePanel::Load()
{
	m_test = new TestC3();
	m_test->Load();
}

}