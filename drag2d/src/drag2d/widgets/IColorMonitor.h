#ifndef _DRAG2D_INTERFACE_COLOR_MONITOR_H_
#define _DRAG2D_INTERFACE_COLOR_MONITOR_H_

namespace d2d
{

class IColorMonitor
{
public:
	virtual void OnColorChanged() = 0;
	virtual ~IColorMonitor() {}

}; // IColorMonitor

}

#endif // _DRAG2D_INTERFACE_COLOR_MONITOR_H_