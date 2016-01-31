#ifndef _EASYEDITOR_COLOR_MONITOR_H_
#define _EASYEDITOR_COLOR_MONITOR_H_

#include "Color.h"

namespace ee
{

class ColorMonitor
{
public:
	virtual Colorf GetColor() const = 0;
	virtual void OnColorChanged() = 0;
	virtual void OnColorChanged(const Colorf& col) = 0;
	virtual ~ColorMonitor() {}

}; // ColorMonitor

}

#endif // _EASYEDITOR_COLOR_MONITOR_H_