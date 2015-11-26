#ifndef _DRAG2D_PROPERTY_COLOR_MONITOR_H_
#define _DRAG2D_PROPERTY_COLOR_MONITOR_H_

#include "widgets/IColorMonitor.h"

namespace d2d
{

class PropertyColorListener : public IColorMonitor
{
public:
	PropertyColorListener(Colorf* col);

	//
	// interface IColorMonitor
	//
	virtual Colorf GetColor() const;
	virtual void OnColorChanged();
	virtual void OnColorChanged(const Colorf& col);

private:
	Colorf* m_col;

}; // PropertyColorListener

}

#endif // _DRAG2D_PROPERTY_COLOR_MONITOR_H_