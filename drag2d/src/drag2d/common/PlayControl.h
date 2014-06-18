#ifndef _DRAG2D_PLAY_CONTROL_H_
#define _DRAG2D_PLAY_CONTROL_H_

namespace d2d
{

class PlayControl
{
public:
	PlayControl(float dt) 
		: m_curr_frame(1), m_last(-1)
	{
		m_dt = dt * CLOCKS_PER_SEC;
	}
	~PlayControl() {
	}

	bool update() {
		bool refresh = false;

		if (m_last == -1) {
			m_last = clock();
		} else {
			clock_t curr = clock();
			int dt = curr - m_last;
			while (dt > m_dt) {
				dt -= m_dt;
				++m_curr_frame;
				refresh = true;
			}
			m_last = curr - dt;
		}

		return refresh;
	}

	void reset() {
		m_curr_frame = 1;
	}

	void decrease() {
		--m_curr_frame;
	}

	int frame() const {
		return m_curr_frame;
	}

private:
	int m_curr_frame;

	clock_t m_last;

	int m_dt;

}; // PlayControl

}

#endif // _DRAG2D_PLAY_CONTROL_H_