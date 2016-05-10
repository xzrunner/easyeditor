#ifndef _EASYEDITOR_PROPERTY_COLOR_MONITOR_H_
#define _EASYEDITOR_PROPERTY_COLOR_MONITOR_H_

#include "ColorMonitor.h"

namespace ee
{

class PropertyColorListener : public ColorMonitor
{
public:
	PropertyColorListener(s2::Color* col);

	//
	// interface ColorMonitor
	//
	virtual s2::Color GetColor() const;
	virtual void OnColorChanged();
	virtual void OnColorChanged(const s2::Color& col);

private:
	s2::Color* m_col;

}; // PropertyColorListener

}

#endif // _EASYEDITOR_PROPERTY_COLOR_MONITOR_H_