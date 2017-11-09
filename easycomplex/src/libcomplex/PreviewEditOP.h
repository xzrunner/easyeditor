#ifndef _EASYCOMPLEX_PREVIEW_EDITOP_H_
#define _EASYCOMPLEX_PREVIEW_EDITOP_H_

#include <ee/ZoomViewOP.h>
#include <ee/Sprite.h>

#include <vector>

namespace ecomplex
{

class PreviewEditOP : public ee::ZoomViewOP
{
public:
	PreviewEditOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		const std::vector<ee::SprPtr>& sprs);

	virtual bool OnMouseLeftDown(int x, int y);

private:
	const std::vector<ee::SprPtr>& m_sprs;

}; // PreviewEditOP

}

#endif // _EASYCOMPLEX_PREVIEW_EDITOP_H_