#include "KeyDownHandler.h"

#include "frame/Controller.h"
#include "view/StagePanel.h"

namespace eanim
{

KeyDownHandler::KeyDownHandler(d2d::AbstractEditOP* editop,
							   Controller* ctrl)
	: m_editop(editop)
	, m_ctrl(ctrl)
{
}

void KeyDownHandler::process(int keyCode) const
{
	if (m_ctrl->GetStagePanel()->GetKeyState(WXK_CONTROL) || 
		m_ctrl->GetStagePanel()->GetKeyState(WXK_SHIFT)) {
		return;
	}

	switch (keyCode)
	{
	case 'z': case 'Z':
		m_ctrl->setPrevKeyFrame();
		m_editop->Clear();
		break;
	case 'x': case 'X':
		m_ctrl->setNextKeyFrame();
		m_editop->Clear();
		break;
	}
}

}