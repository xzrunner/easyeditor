#ifndef _EASYANIM_PREVIEW_OP_H_
#define _EASYANIM_PREVIEW_OP_H_

#include <ee/ZoomViewOP.h>

#include <sprite2/s2_config.h>

namespace s2 { class AnimTreeCurr; }

namespace eanim
{

class PreviewOP : public ee::ZoomViewOP
{
public:
	PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, s2::AnimTreeCurr& curr);

	virtual bool OnMouseLeftDown(int x, int y);

private:
	s2::AnimTreeCurr& m_curr;

}; // PreviewOP

}

#endif // _EASYANIM_PREVIEW_OP_H_