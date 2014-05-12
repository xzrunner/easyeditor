#include "KeyDownHandler.h"

#include "frame/Context.h"

namespace eanim
{

KeyDownHandler::KeyDownHandler(d2d::AbstractEditOP* editop)
	: m_editop(editop)
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
		Context::Instance()->setPrevKeyFrame();
		m_editop->clear();
		break;
	case 'x': case 'X':
		Context::Instance()->setNextKeyFrame();
		m_editop->clear();
		break;
	}
}

}