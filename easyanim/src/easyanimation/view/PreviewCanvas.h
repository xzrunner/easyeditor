#ifndef _EASYANIM_PREVIEW_CANVAS_H_
#define _EASYANIM_PREVIEW_CANVAS_H_

#include <ee/CameraCanvas.h>

#include <sprite2/s2_config.h>

#include <wx/wx.h>

#ifdef S2_ANIM_CURR_OLD
namespace s2 { class AnimCurr; }
#else
namespace s2 { class AnimCurr2; }
#endif // S2_ANIM_CURR_OLD

namespace eanim
{

class PreviewCanvas : public ee::CameraCanvas
{
public:
#ifdef S2_ANIM_CURR_OLD
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, wxGLContext* glctx, s2::AnimCurr& curr);
#else
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, wxGLContext* glctx, s2::AnimCurr2& curr);
#endif // S2_ANIM_CURR_OLD

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawStageData() const;

private:
#ifdef S2_ANIM_CURR_OLD
	s2::AnimCurr& m_curr;
#else
	s2::AnimCurr2& m_curr;
#endif // S2_ANIM_CURR_OLD

}; // PreviewCanvas

}

#endif // _EASYANIM_PREVIEW_CANVAS_H_