#ifndef _EASYANIM_PREVIEW_OP_H_
#define _EASYANIM_PREVIEW_OP_H_

#include <ee/ZoomViewOP.h>

#ifdef S2_ANIM_CURR_OLD
namespace s2 { class AnimCurr; }
#else
namespace s2 { class AnimCurr2; }
#endif // S2_ANIM_CURR_OLD

namespace eanim
{

class PreviewOP : public ee::ZoomViewOP
{
public:
#ifdef S2_ANIM_CURR_OLD
	PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimCurr& curr);
#else
	PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimCurr2& curr);
#endif // S2_ANIM_CURR_OLD

	virtual bool OnMouseLeftDown(int x, int y);

private:
#ifdef S2_ANIM_CURR_OLD
	s2::AnimCurr& m_curr;
#else
	s2::AnimCurr2& m_curr;
#endif // S2_ANIM_CURR_OLD

}; // PreviewOP

}

#endif // _EASYANIM_PREVIEW_OP_H_