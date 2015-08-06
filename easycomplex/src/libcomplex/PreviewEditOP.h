#ifndef _EASYCOMPLEX_PREVIEW_EDITOP_H_
#define _EASYCOMPLEX_PREVIEW_EDITOP_H_

#include <drag2d.h>

namespace ecomplex
{

class PreviewEditOP : public d2d::ZoomViewOP
{
public:
	PreviewEditOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
		const std::vector<const d2d::ISprite*>& sprites);

	virtual bool OnMouseLeftDown(int x, int y);

private:
	const std::vector<const d2d::ISprite*>& m_sprites;

}; // PreviewEditOP

}

#endif // _EASYCOMPLEX_PREVIEW_EDITOP_H_