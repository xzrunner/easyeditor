#ifndef _EASYPARTICLE2D_UI_CALLBACK_H_
#define _EASYPARTICLE2D_UI_CALLBACK_H_

namespace eparticle2d
{

class UICallback
{
public:
	virtual ~UICallback() {}
	virtual void OnSetKeyValue(int key, int val) = 0;
	virtual void OnSetKeyValue(int key, int val0, int val1) = 0;

}; // UICallback

}

#endif // _EASYPARTICLE2D_UI_CALLBACK_H_