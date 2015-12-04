#ifndef _EASYANIM_KEY_DOWN_HANDLER_H_
#define _EASYANIM_KEY_DOWN_HANDLER_H_

namespace eanim
{

class KeyDownHandler
{
public:
	void Process(int key_code);

private:
	void SetPrevKeyFrame();
	void SetNextKeyFrame();

}; // KeyDownHandler

}

#endif // _EASYANIM_KEY_DOWN_HANDLER_H_