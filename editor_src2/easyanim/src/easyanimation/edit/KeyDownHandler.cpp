#include "KeyDownHandler.h"

#include "frame/Context.h"

namespace eanim
{

void KeyDownHandler::process(int keyCode) const
{
	switch (keyCode)
	{
	case 'z': case 'Z':
		Context::Instance()->setPrevKeyFrame();
		break;
	case 'x': case 'X':
		Context::Instance()->setNextKeyFrame();
		break;
	}
}

}