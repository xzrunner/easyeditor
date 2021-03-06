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

	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseLeftDClick(int x, int y) override;

protected:
	virtual void onDirectionKeyDown(int type) override;

	virtual void PasteSprToClipboard(const ee::SprConstPtr& spr, Json::Value& value) const override;
	virtual void CopySprFromClipboard(const ee::SprPtr& spr, const Json::Value& value) const override;

}; // ArrangeSpriteOP

}

#endif // _SG_ARRANGE_SPRITE_OP_H_