#ifndef _SG_ARRANGE_SPRITE_OP_H_
#define _SG_ARRANGE_SPRITE_OP_H_

#include <drag2d.h>

namespace sg
{

class StagePanel;

class ArrangeSpriteOP : public d2d::ArrangeSpriteFixOP
{
public:
	ArrangeSpriteOP(d2d::PropertySettingPanel* property, StagePanel* stage);

	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);

protected:
	virtual void onDirectionKeyDown(d2d::DirectionType type);

	virtual void PasteSprToClipboard(const d2d::ISprite* spr, Json::Value& value) const;
	virtual void CopySprFromClipboard(d2d::ISprite* spr, const Json::Value& value) const;

}; // ArrangeSpriteOP

}

#endif // _SG_ARRANGE_SPRITE_OP_H_