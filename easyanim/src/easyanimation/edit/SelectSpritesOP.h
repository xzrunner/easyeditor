#ifndef _EASYANIM_SELECT_SPRITES_OP_H_
#define _EASYANIM_SELECT_SPRITES_OP_H_

#include "OpenSymbolDialog.h"

#include <ee/SelectSpritesOP.h>

namespace eanim
{

class SelectSpritesOP : public ee::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* sprites_impl, 
		ee::EditCMPT* callback = NULL);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDClick(int x, int y) override;

protected:
	virtual void PasteSprToClipboard(const ee::SprConstPtr& spr, Json::Value& value) const override;
	virtual void CopySprFromClipboard(const ee::SprPtr& spr, const Json::Value& value) const override;

private:
	OpenSymbolDialog m_open_symbol;

}; // SelectSpritesOP

}

#endif // _EASYANIM_SELECT_SPRITES_OP_H_