#ifndef _EASYEDITOR_COLOR_MONITOR_H_
#define _EASYEDITOR_COLOR_MONITOR_H_

#include <painting2/Color.h>

namespace ee
{

class ColorMonitor
{
public:
	virtual pt2::Color GetColor() const = 0;
	virtual void OnColorChanged() = 0;
	virtual void OnColorChanged(const pt2::Color& col) = 0;
	virtual ~ColorMonitor() {}

}; // ColorMonitor

}

#endif // _EASYEDITOR_COLOR_MONITOR_H_