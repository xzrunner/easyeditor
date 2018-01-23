#ifndef _EASYEDITOR_SELECT_DRAW_TYPE_WIDGET_H_
#define _EASYEDITOR_SELECT_DRAW_TYPE_WIDGET_H_

#include "Widget.h"

#include <memory>

namespace ee
{

class CameraCanvas;

class SelectDrawTypeWidget : public Widget
{
public:
	SelectDrawTypeWidget(wxWindow* parent, const std::shared_ptr<CameraCanvas>& canvas);

private:
	void OnSetDrawType(wxCommandEvent& event);

private:
	std::shared_ptr<CameraCanvas> m_canvas = nullptr;

}; // SelectDrawTypeWidget

}

#endif // _EASYEDITOR_SELECT_DRAW_TYPE_WIDGET_H_