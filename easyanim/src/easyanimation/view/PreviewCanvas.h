#ifndef _EASYANIM_PREVIEW_CANVAS_H_
#define _EASYANIM_PREVIEW_CANVAS_H_

#include <ee/CameraCanvas.h>

#include <sprite2/s2_config.h>

#include <wx/wx.h>

#ifdef S2_ANIM_CURR_V0
namespace s2 { class AnimCurr; }
#elif defined S2_ANIM_CURR_V1
namespace s2 { class AnimCurr2; }
#elif defined S2_ANIM_CURR_V2
namespace s2 { class AnimCurr3; }
#endif

namespace eanim
{

class PreviewCanvas : public ee::CameraCanvas
{
public:
#ifdef S2_ANIM_CURR_V0
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, wxGLContext* glctx, s2::AnimCurr& curr);
#elif defined S2_ANIM_CURR_V1
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, wxGLContext* glctx, s2::AnimCurr2& curr);
#elif defined S2_ANIM_CURR_V2
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, wxGLContext* glctx, s2::AnimCurr3& curr);
#endif

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawStageData() const;

private:
#ifdef S2_ANIM_CURR_V0
	s2::AnimCurr& m_curr;
#elif defined S2_ANIM_CURR_V1
	s2::AnimCurr2& m_curr;
#elif defined S2_ANIM_CURR_V2
	s2::AnimCurr3& m_curr;
#endif

}; // PreviewCanvas

}

#endif // _EASYANIM_PREVIEW_CANVAS_H_