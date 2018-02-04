#include "ee0/KeysState.h"

#include <string.h>

namespace ee0
{

KeysState::KeysState()
{
	Reset();
}

void KeysState::OnKeyDown(int key)
{
	if (key < CAPACITY) {
		m_keys_state[key] = true;
	}
}

void KeysState::OnKeyUp(int key)
{
	if (key < CAPACITY) {
		m_keys_state[key] = false;
	}
}

void KeysState::Reset()
{
	memset(&m_keys_state[0], 0, sizeof(m_keys_state));
}

}