#ifndef _EASYEDITOR_TWEEN_COLOR_PANEL_H_
#define _EASYEDITOR_TWEEN_COLOR_PANEL_H_

#include <painting2/Color.h>

namespace ee
{

class TweenColorPanel
{
public:
	virtual ~TweenColorPanel() {}
	virtual void SetColor(const pt2::Color& begin, const pt2::Color& end) = 0;

}; // TweenColorPanel

}

#endif // _EASYEDITOR_TWEEN_COLOR_PANEL_H_