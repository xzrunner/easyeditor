#ifndef _EASYEDITOR_PROP_COL_MONITOR_H_
#define _EASYEDITOR_PROP_COL_MONITOR_H_

#include "ColorMonitor.h"

namespace ee
{

class PropColMonitor : public ColorMonitor
{
public:
	PropColMonitor(s2::Color* col);

	//
	// interface ColorMonitor
	//
	virtual s2::Color GetColor() const;
	virtual void OnColorChanged();
	virtual void OnColorChanged(const s2::Color& col);

private:
	s2::Color* m_col;

}; // PropColMonitor

}

#endif // _EASYEDITOR_PROP_COL_MONITOR_H_