#ifndef _SG_ARRANGE_SPRITE_OP_H_
#define _SG_ARRANGE_SPRITE_OP_H_

#include <ee/ArrangeSpriteFixOP.h>

namespace sg
{

class StagePanel;

class ArrangeSpriteOP : public ee::ArrangeSpriteFixOP
{
public:
	ArrangeSpriteOP(ee::PropertySettingPanel* property, StagePanel* stage);

	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);

protected:
	virtual void onDirectionKeyDown(int type);

	virtual void PasteSprToClipboard(const ee::Sprite* spr, Json::Value& value) const;
	virtual void CopySprFromClipboard(ee::Sprite* spr, const Json::Value& value) const;

}; // ArrangeSpriteOP

}

#endif // _SG_ARRANGE_SPRITE_OP_H_