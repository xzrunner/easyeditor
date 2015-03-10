#ifndef _DRAG2D_I_TWEEN_COLOR_PANEL_H_
#define _DRAG2D_I_TWEEN_COLOR_PANEL_H_

namespace d2d
{

struct Colorf;

class ITweenColorPanel
{
public:
	virtual ~ITweenColorPanel() {}
	virtual void SetColor(const Colorf& begin, const Colorf& end) = 0;

}; // ITweenColorPanel

}

#endif // _DRAG2D_I_TWEEN_COLOR_PANEL_H_