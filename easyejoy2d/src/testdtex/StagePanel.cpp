#include "StagePanel.h"
#include "StageCanvas.h"

#include "TestC3.h"
#include "TestDSprite.h"

#include <easyejoy2d.h>

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
	eejoy2d::EJScreen::Create();

//	m_test = new TestC3();
	m_test = new TestDSprite();
	m_test->Load();
}

void StagePanel::UnLoad()
{
	assert(eejoy2d::EJScreen::Instance());
	eejoy2d::EJScreen::Instance()->Unload();
}

void StagePanel::Update()
{
	if (m_test) {
		m_test->Update();
	}
}

void StagePanel::Draw() const
{
	if (m_test) {
		m_test->Draw();
	}
}

}