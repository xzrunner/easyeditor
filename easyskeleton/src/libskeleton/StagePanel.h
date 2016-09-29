#ifndef _LIBSKELETON_STAGE_PANEL_H_
#define _LIBSKELETON_STAGE_PANEL_H_

#include <ee/EditPanel.h>

class wxGLContext;

namespace libskeleton
{

class Sprite;

class StagePanel : public ee::EditPanel
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		wxGLContext* glctx, Sprite* spr);
	virtual ~StagePanel();

	//
	// ee::EditPanel interface
	//
	virtual bool UpdateStage();

	const Sprite* GetSprite() const { return m_spr; }

private:
	Sprite* m_spr;

}; // StagePanel

}

#endif // _LIBSKELETON_STAGE_PANEL_H_