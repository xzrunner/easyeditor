#ifndef _EASYANIM_KEY_DOWN_HANDLER_H_
#define _EASYANIM_KEY_DOWN_HANDLER_H_

namespace eanim
{

class KeyDownHandler
{
public:
	bool Process(int key_code);

	static KeyDownHandler* Instance();

private:
	KeyDownHandler() {}

	void SetPrevKeyFrame();
	void SetNextKeyFrame();

private:
	static KeyDownHandler* m_instance;

}; // KeyDownHandler

}

#endif // _EASYANIM_KEY_DOWN_HANDLER_H_