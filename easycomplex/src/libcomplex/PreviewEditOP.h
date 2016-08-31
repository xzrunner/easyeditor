#ifndef _EASYCOMPLEX_PREVIEW_EDITOP_H_
#define _EASYCOMPLEX_PREVIEW_EDITOP_H_

#include <ee/ZoomViewOP.h>

#include <vector>

namespace ee { class Sprite; }

namespace ecomplex
{

class PreviewEditOP : public ee::ZoomViewOP
{
public:
	PreviewEditOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		const std::vector<ee::Sprite*>& sprs);

	virtual bool OnMouseLeftDown(int x, int y);

private:
	const std::vector<ee::Sprite*>& m_sprs;

}; // PreviewEditOP

}

#endif // _EASYCOMPLEX_PREVIEW_EDITOP_H_