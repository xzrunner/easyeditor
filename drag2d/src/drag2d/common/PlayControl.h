#ifndef _DRAG2D_PLAY_CONTROL_H_
#define _DRAG2D_PLAY_CONTROL_H_

#include <time.h>

namespace d2d
{

class PlayControl
{
public:
	PlayControl(float dt);
	~PlayControl();

	bool update();

	void reset();

	void decrease();

	int frame() const;

	void SetDt(float dt);

private:
	int m_curr_frame;

	clock_t m_last;
	clock_t m_init;

	int m_dt;

}; // PlayControl

}

#endif // _DRAG2D_PLAY_CONTROL_H_