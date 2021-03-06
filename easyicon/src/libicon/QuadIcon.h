#ifndef _EASYICON_QUAD_ICON_H_
#define _EASYICON_QUAD_ICON_H_

#include "Icon.h"

#include <sprite2/StaticQuadIcon.h>

#include <json/json.h>

namespace eicon
{

class QuadIcon : public Icon, public s2::StaticQuadIcon
{
public:
	QuadIcon() {}
	QuadIcon(const std::shared_ptr<s2::ImageSymbol>& img, 
		const sm::vec2* src, const sm::vec2* screen);

	/**
	 *  @interface
	 *    Icon
	 */
	virtual const char* GetIconDesc() const override { return "quad"; }

	virtual void LoadFromFile(const Json::Value& value) override;
	virtual void StoreToFile(Json::Value& value) const override;
	virtual void LoadFromFile(const s2s::IconSym& sym) override;

}; // QuadIcon

}

#endif // _EASYICON_QUAD_ICON_H_