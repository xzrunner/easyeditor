#include "KeyDownHandler.h"

#include "frame/Controller.h"

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
	if (wxGetKeyState(WXK_CONTROL) || wxGetKeyState(WXK_SHIFT)) {
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