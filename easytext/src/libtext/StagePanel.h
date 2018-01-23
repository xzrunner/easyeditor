#ifndef _EASYTEXT_STAGE_PANEL_H_
#define _EASYTEXT_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>

class wxGLContext;

namespace etext
{

class StageCanvas;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, ee::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, wxGLContext* glctx,
		ee::SprPtr edited, const ee::MultiSpritesImpl* bg_sprites, 
		ee::LibraryPanel* library);
	virtual ~StagePanel();

private:
	std::shared_ptr<StageCanvas> m_canvas = nullptr;

}; // StagePanel

}

#endif // _EASYTEXT_STAGE_PANEL_H_