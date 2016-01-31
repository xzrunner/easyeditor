#ifndef _EASYEDITOR_KEY_STATE_H_
#define _EASYEDITOR_KEY_STATE_H_

namespace ee
{

class KeysState
{
public:
	KeysState();

	void OnKeyDown(int key);
	void OnKeyUp(int key);

	void Reset();

	bool GetKeyState(int key) const { 
		return m_keys_state[key]; 
	}

private:
	static const int CAPACITY = 512;

private:
	bool m_keys_state[512];

}; // KeysState

}

#endif // _EASYEDITOR_KEY_STATE_H_