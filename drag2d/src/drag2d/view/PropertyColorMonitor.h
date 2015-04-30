#ifndef _DRAG2D_PROPERTY_COLOR_MONITOR_H_
#define _DRAG2D_PROPERTY_COLOR_MONITOR_H_

#include "widgets/IColorMonitor.h"

namespace d2d
{

class EditPanel;

class PropertyColorListener : public IColorMonitor
{
public:
	PropertyColorListener(EditPanel* stage, Colorf* col);

	//
	// interface IColorMonitor
	//
	virtual Colorf GetColor() const;
	virtual void OnColorChanged();
	virtual void OnColorChanged(const Colorf& col);

private:
	EditPanel* m_stage;

	Colorf* m_col;

}; // PropertyColorListener

}

#endif // _DRAG2D_PROPERTY_COLOR_MONITOR_H_