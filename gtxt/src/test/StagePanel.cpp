#include "StagePanel.h"
#include "StageCanvas.h"
#include "TestNormal.h"

#include <easyejoy2d.h>
#include <dtex.h>

namespace tgtxt
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
{
	SetEditOP(new d2d::ZoomViewOP(this, GetStageImpl(), true));
	SetCanvas(new StageCanvas(this));
}

StagePanel::~StagePanel()
{

}

void StagePanel::Load()
{
	eejoy2d::EJScreen::Create();

	m_test = new TestNormal();

	m_test->Load();
}

void StagePanel::UnLoad()
{
	assert(eejoy2d::EJScreen::Instance());
	eejoy2d::EJScreen::Instance()->Unload();
}

void StagePanel::Update()
{
}

void StagePanel::Draw() const
{
	m_test->Draw();
}

}