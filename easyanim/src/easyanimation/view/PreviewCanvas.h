#ifndef _EASYANIM_PREVIEW_CANVAS_H_
#define _EASYANIM_PREVIEW_CANVAS_H_

#include <ee/OrthoCanvas.h>

#include <wx/wx.h>

namespace ee { class PlayControl; }

class PlaySettings;

namespace eanim
{

struct PlaySettings;

class PreviewCanvas : public ee::OrthoCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		const PlaySettings& settings, ee::PlayControl& control,
		wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	void DrawStageData() const;

private:
	ee::PlayControl& m_control;

	const PlaySettings& m_settings;

}; // PreviewCanvas

}

#endif // _EASYANIM_PREVIEW_CANVAS_H_