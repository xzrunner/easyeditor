#ifndef _EASYEDITOR_PLAY_CONTROL_H_
#define _EASYEDITOR_PLAY_CONTROL_H_

#include <time.h>

namespace ee
{

class PlayControl
{
public:
	PlayControl(float dt);
	~PlayControl();

	bool Update();

	void Reset();

	void Decrease();

	int Frame() const;

	void SetDt(float dt);

private:
	int m_curr_frame;

	clock_t m_last;
	clock_t m_init;

	int m_dt;

}; // PlayControl

}

#endif // _EASYEDITOR_PLAY_CONTROL_H_