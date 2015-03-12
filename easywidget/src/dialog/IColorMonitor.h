#ifndef _EASYWIDGET_ICOLOR_MONITOR_H_
#define _EASYWIDGET_ICOLOR_MONITOR_H_

namespace ewidget
{

class IColorMonitor
{
public:
	virtual void OnColorChanged() = 0;
	virtual ~IColorMonitor() {}

}; // IColorMonitor

}

#endif // _EASYWIDGET_ICOLOR_MONITOR_H_