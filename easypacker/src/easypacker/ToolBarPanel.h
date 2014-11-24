#ifndef _EPACKER_TOOLBAR_PANEL_H_
#define _EPACKER_TOOLBAR_PANEL_H_

#include <drag2d.h>

#include "const.h"

namespace epacker
{

class StagePanel;
class ImageToolbarPage;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

	IMG_TYPE getImgType() const;

	void setSize(int width, int height);

protected:
	virtual wxSizer* initLayout();

private:
	StagePanel* m_stage;

	ImageToolbarPage* m_image_page;

}; // ToolbarPanel

}

#endif // _EPACKER_TOOLBAR_PANEL_H_