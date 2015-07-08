#ifndef _DRAG2D_I_REFRESH_MONITOR_H_
#define _DRAG2D_I_REFRESH_MONITOR_H_

namespace d2d
{

class IRefreshMonitor
{
public:
	virtual ~IRefreshMonitor() {}
	virtual void OnRefresh() = 0;
}; // IRefreshMonitor

}

#endif // _DRAG2D_I_REFRESH_MONITOR_H_