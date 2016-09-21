#ifndef _EASYICON_QUAD_ICON_H_
#define _EASYICON_QUAD_ICON_H_

#include "Icon.h"

#include <sprite2/StaticQuadIcon.h>

#include <json/json.h>

namespace ee { class ImageSymbol; }

namespace eicon
{

class QuadIcon : public Icon, public s2::StaticQuadIcon
{
public:
	QuadIcon() {}
	QuadIcon(ee::ImageSymbol* img, const sm::vec2* src, 
		const sm::vec2* screen);

	/**
	 *  @interface
	 *    Icon
	 */
	virtual const char* GetIconDesc() const { return "quad"; }

	virtual void LoadFromFile(const Json::Value& value);
	virtual void StoreToFile(Json::Value& value) const;

}; // QuadIcon

}

#endif // _EASYICON_QUAD_ICON_H_