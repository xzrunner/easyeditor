#ifndef _EASYEDITOR_TWEEN_COLOR_PANEL_H_
#define _EASYEDITOR_TWEEN_COLOR_PANEL_H_

namespace ee
{

class Colorf;

class TweenColorPanel
{
public:
	virtual ~TweenColorPanel() {}
	virtual void SetColor(const Colorf& begin, const Colorf& end) = 0;

}; // TweenColorPanel

}

#endif // _EASYEDITOR_TWEEN_COLOR_PANEL_H_