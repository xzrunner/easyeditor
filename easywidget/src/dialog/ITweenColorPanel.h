#ifndef _EASYWIDGET_ITWEEN_COLOR_PANEL_H_
#define _EASYWIDGET_ITWEEN_COLOR_PANEL_H_

struct Colorf;

namespace ewidget
{

class ITweenColorPanel
{
public:
	virtual ~ITweenColorPanel() {}
	virtual void SetColor(const Colorf& begin, const Colorf& end) = 0;

}; // ITweenColorPanel

}

#endif // _EASYWIDGET_I_TWEEN_COLOR_PANEL_H_
