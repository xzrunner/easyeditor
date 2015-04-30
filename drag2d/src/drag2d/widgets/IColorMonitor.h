#ifndef _DRAG2D_INTERFACE_COLOR_MONITOR_H_
#define _DRAG2D_INTERFACE_COLOR_MONITOR_H_

namespace d2d
{

struct Colorf;

class IColorMonitor
{
public:
	virtual Colorf GetColor() const = 0;
	virtual void OnColorChanged() = 0;
	virtual void OnColorChanged(const Colorf& col) = 0;
	virtual ~IColorMonitor() {}

}; // IColorMonitor

}

#endif // _DRAG2D_INTERFACE_COLOR_MONITOR_H_