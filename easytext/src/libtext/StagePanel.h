#ifndef _EASYTEXT_STAGE_PANEL_H_
#define _EASYTEXT_STAGE_PANEL_H_

#include <drag2d.h>

namespace etext
{

class StageCanvas;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, d2d::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, wxGLContext* glctx,
		d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites, 
		d2d::LibraryPanel* library);
	virtual ~StagePanel();

private:
	StageCanvas* m_canvas;

}; // StagePanel

}

#endif // _EASYTEXT_STAGE_PANEL_H_