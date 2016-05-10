#ifndef _EASYEDITOR_TWEEN_COLOR_PANEL_H_
#define _EASYEDITOR_TWEEN_COLOR_PANEL_H_

#include <sprite2/Color.h>

namespace ee
{

class TweenColorPanel
{
public:
	virtual ~TweenColorPanel() {}
	virtual void SetColor(const s2::Color& begin, const s2::Color& end) = 0;

}; // TweenColorPanel

}

#endif // _EASYEDITOR_TWEEN_COLOR_PANEL_H_