#ifndef _EASYEDITOR_PROPERTY_COLOR_MONITOR_H_
#define _EASYEDITOR_PROPERTY_COLOR_MONITOR_H_

#include "ColorMonitor.h"

namespace ee
{

class PropertyColorListener : public ColorMonitor
{
public:
	PropertyColorListener(Colorf* col);

	//
	// interface ColorMonitor
	//
	virtual Colorf GetColor() const;
	virtual void OnColorChanged();
	virtual void OnColorChanged(const Colorf& col);

private:
	Colorf* m_col;

}; // PropertyColorListener

}

#endif // _EASYEDITOR_PROPERTY_COLOR_MONITOR_H_