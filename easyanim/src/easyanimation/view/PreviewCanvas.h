#ifndef _EASYANIM_PREVIEW_CANVAS_H_
#define _EASYANIM_PREVIEW_CANVAS_H_

#include <ee/CameraCanvas.h>

#include <wx/wx.h>

namespace s2 { class AnimCurr; }

namespace eanim
{

class PreviewCanvas : public ee::CameraCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		wxGLContext* glctx, s2::AnimCurr& curr);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawStageData() const;

private:
	s2::AnimCurr& m_curr;

}; // PreviewCanvas

}

#endif // _EASYANIM_PREVIEW_CANVAS_H_