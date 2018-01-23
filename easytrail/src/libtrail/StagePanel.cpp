#include "StagePanel.h"
#include "StageCanvas.h"
#include "MotionTrail.h"
#include "EditOP.h"

namespace etrail
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library,
					   ee::PropertySettingPanel* property)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, m_trail(NULL)
{
	SetEditOP(std::make_shared<EditOP>(this));
	SetCanvas(std::make_shared<StageCanvas>(this));
}

StagePanel::~StagePanel()
{	
}

bool StagePanel::UpdateStage()
{
	if (m_trail) {
		return m_trail->Update(m_pos);
	} else {
		return false;
	}
}

}