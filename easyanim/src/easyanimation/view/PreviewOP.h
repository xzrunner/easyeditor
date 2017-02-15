#ifndef _EASYANIM_PREVIEW_OP_H_
#define _EASYANIM_PREVIEW_OP_H_

#include <ee/ZoomViewOP.h>

#include <sprite2/s2_config.h>

#ifdef S2_ANIM_CURR_V0
namespace s2 { class AnimCurr; }
#elif defined S2_ANIM_CURR_V1
namespace s2 { class AnimCurr2; }
#elif defined S2_ANIM_CURR_V2
namespace s2 { class AnimCurr3; }
#endif

namespace eanim
{

class PreviewOP : public ee::ZoomViewOP
{
public:
#ifdef S2_ANIM_CURR_V0
	PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimCurr& curr);
#elif defined S2_ANIM_CURR_V1
	PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimCurr2& curr);
#elif defined S2_ANIM_CURR_V2
	PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimCurr3& curr);
#endif

	virtual bool OnMouseLeftDown(int x, int y);

private:
#ifdef S2_ANIM_CURR_V0
	s2::AnimCurr& m_curr;
#elif defined S2_ANIM_CURR_V1
	s2::AnimCurr2& m_curr;
#elif defined S2_ANIM_CURR_V2
	s2::AnimCurr3& m_curr;
#endif

}; // PreviewOP

}

#endif // _EASYANIM_PREVIEW_OP_H_