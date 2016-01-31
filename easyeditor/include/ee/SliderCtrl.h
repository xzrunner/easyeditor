#ifndef _EASYEDITOR_SLIDER_CTRL_H_
#define _EASYEDITOR_SLIDER_CTRL_H_

#include <json/json.h>

namespace ee
{

class SliderCtrl
{
public:
	virtual void Update() = 0;
	virtual void Load(const Json::Value& val, int version) = 0;
	virtual void Store(Json::Value& val) = 0;
	virtual void Load() = 0;
	virtual ~SliderCtrl() {}
}; // SliderCtrl

}

#endif // _EASYEDITOR_SLIDER_CTRL_H_