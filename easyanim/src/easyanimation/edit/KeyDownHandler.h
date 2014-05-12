#ifndef _EASYANIM_KEY_DOWN_HANDLER_H_
#define _EASYANIM_KEY_DOWN_HANDLER_H_

#include <drag2d.h>

namespace eanim
{

class KeyDownHandler
{
public:
	KeyDownHandler(d2d::AbstractEditOP* editop);

	void process(int keyCode) const;

private:
	d2d::AbstractEditOP* m_editop;

}; // KeyDownHandler

}

#endif // _EASYANIM_KEY_DOWN_HANDLER_H_