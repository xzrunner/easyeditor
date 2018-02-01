#ifndef _EASYEDITOR_PROP_COL_MONITOR_H_
#define _EASYEDITOR_PROP_COL_MONITOR_H_

#include "ColorMonitor.h"

namespace ee
{

class PropColMonitor : public ColorMonitor
{
public:
	PropColMonitor(pt2::Color* col);

	//
	// interface ColorMonitor
	//
	virtual pt2::Color GetColor() const override;
	virtual void OnColorChanged() override;
	virtual void OnColorChanged(const pt2::Color& col) override;

private:
	pt2::Color* m_col;

}; // PropColMonitor

}

#endif // _EASYEDITOR_PROP_COL_MONITOR_H_