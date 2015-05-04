#ifndef _EASYANIM_KEY_DOWN_HANDLER_H_
#define _EASYANIM_KEY_DOWN_HANDLER_H_

#include <drag2d.h>

namespace eanim
{

class Controller;

class KeyDownHandler
{
public:
	KeyDownHandler(d2d::AbstractEditOP* editop,
		Controller* ctrl);

	void process(int keyCode) const;

private:
	d2d::AbstractEditOP* m_editop;

	Controller* m_ctrl;

}; // KeyDownHandler

}

#endif // _EASYANIM_KEY_DOWN_HANDLER_H_