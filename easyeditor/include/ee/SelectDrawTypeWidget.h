#ifndef _EASYEDITOR_SELECT_DRAW_TYPE_WIDGET_H_
#define _EASYEDITOR_SELECT_DRAW_TYPE_WIDGET_H_

#include "Widget.h"

namespace ee
{

class CameraCanvas;

class SelectDrawTypeWidget : public Widget
{
public:
	SelectDrawTypeWidget(wxWindow* parent, CameraCanvas* canvas);

private:
	void OnSetDrawType(wxCommandEvent& event);

private:
	CameraCanvas* m_canvas;

}; // SelectDrawTypeWidget

}

#endif // _EASYEDITOR_SELECT_DRAW_TYPE_WIDGET_H_