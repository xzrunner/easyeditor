#ifndef _EASYANIM_PREVIEW_OP_H_
#define _EASYANIM_PREVIEW_OP_H_

#include <ee/ZoomViewOP.h>

namespace s2 { class AnimCurr; }

namespace eanim
{

class PreviewOP : public ee::ZoomViewOP
{
public:
	PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		s2::AnimCurr& curr);

	virtual bool OnMouseLeftDown(int x, int y);

private:
	s2::AnimCurr& m_curr;

}; // PreviewOP

}

#endif // _EASYANIM_PREVIEW_OP_H_