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
	ee::EditOP* op = new EditOP(this);
	SetEditOP(op);
	op->RemoveReference();

	ee::StageCanvas* canvas = new StageCanvas(this);
	SetCanvas(canvas);
	canvas->RemoveReference();
}

StagePanel::~StagePanel()
{	
}

bool StagePanel::Update(float dt)
{
	TrailNode::Instance()->UpdateTime();
	if (m_trail) {
		return m_trail->Update(m_pos);
	} else {
		return false;
	}
}

}