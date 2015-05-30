#ifndef _DRAG2D_INTERFACE_SLIDER_CTRL_H_
#define _DRAG2D_INTERFACE_SLIDER_CTRL_H_

#include <json/json.h>

namespace d2d
{

class ISliderCtrl
{
public:
	virtual void Update() = 0;
	virtual void Load(const Json::Value& val, int version) = 0;
	virtual void Store(Json::Value& val) = 0;
	virtual void Load() = 0;
	virtual ~ISliderCtrl() {}
}; // ISliderCtrl

}

#endif // _DRAG2D_INTERFACE_SLIDER_CTRL_H_