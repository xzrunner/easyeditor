#ifndef _EASYEDITOR_UI_CALLBACK_H_
#define _EASYEDITOR_UI_CALLBACK_H_

namespace ee
{

class UICallback
{
public:
	struct Data
	{
		float val0;
		float val1;

		Data() {}
		Data(float val) : val0(val) {}
		Data(float val0, float val1) : val0(val0), val1(val1) {}
	};

public:
	virtual ~UICallback() {}
	virtual void SetValue(int key, const Data& data) = 0;
	virtual void GetValue(int key, Data& data) = 0;

}; // UICallback

}

#endif // _EASYEDITOR_UI_CALLBACK_H_