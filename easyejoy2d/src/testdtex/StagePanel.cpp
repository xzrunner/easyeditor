#include "StagePanel.h"
#include "StageCanvas.h"

#include "TestC3.h"
#include "TestC2.h"
#include "TestC2C3.h"
#include "TestSprite.h"
#include "TestPVR.h"
#include "TestRRP_C2C3.h"
#include "TestRRP_C2C3_PVR.h"
#include "TestRRR_C3.h"
#include "TestB4R_C3.h"
#include "TestETC1.h"
#include "TestEPP.h"

#include <easyejoy2d.h>

namespace tdtex
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, m_test(NULL)
{
	m_edit_op = new d2d::ZoomViewOP(this, true);
	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
	delete m_test;
}

void StagePanel::Clear()
{
}

void StagePanel::Load()
{
	eejoy2d::EJScreen::Create();

//	m_test = new TestC3();
//	m_test = new TestC2();
	m_test = new TestC2C3();

//	m_test = new TestSprite();

//	m_test = new TestPVR();

//	m_test = new TestRRP_C2C3();
//	m_test = new TestRRP_C2C3_PVR();

//	m_test = new TestRRR_C3();
//	m_test = new TestB4R_C3();

//	m_test = new TestETC1();

//	m_test = new TestEPP();

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