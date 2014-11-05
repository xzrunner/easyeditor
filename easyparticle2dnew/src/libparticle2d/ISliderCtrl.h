#ifndef _EASYPARTICLE2D_INTERFACE_SLIDER_CTRL_H_
#define _EASYPARTICLE2D_INTERFACE_SLIDER_CTRL_H_

#include <drag2d.h>

namespace eparticle2d
{

class ISliderCtrl
{
public:
	virtual void Update() = 0;
	virtual void Load(const Json::Value& val) = 0;
	virtual void Store(Json::Value& val) = 0;
	virtual void Load() = 0;
	virtual ~ISliderCtrl() {}
}; // ISliderCtrl

}

#endif // _EASYPARTICLE2D_INTERFACE_SLIDER_CTRL_H_