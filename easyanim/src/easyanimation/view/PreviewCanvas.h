#ifndef _EASYANIM_PREVIEW_CANVAS_H_
#define _EASYANIM_PREVIEW_CANVAS_H_

#include <ee/CameraCanvas.h>

#include <sprite2/s2_config.h>

#include <wx/wx.h>

namespace s2 { class AnimTreeCurr; }

namespace eanim
{

class PreviewCanvas : public ee::CameraCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, wxGLContext* glctx, s2::AnimTreeCurr& curr);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawStageData() const;

private:
	s2::AnimTreeCurr& m_curr;

}; // PreviewCanvas

}

#endif // _EASYANIM_PREVIEW_CANVAS_H_