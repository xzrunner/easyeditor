#ifndef _EASYANIM_PREVIEW_CONTROL_H_
#define _EASYANIM_PREVIEW_CONTROL_H_

namespace eanim
{

class PreviewControl
{
public:
	PreviewControl(float dt) 
		: m_curr_frame(1), m_last(-1)
	{
		m_dt = dt * CLOCKS_PER_SEC;
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

}; // PreviewControl

}

#endif // _EASYANIM_PREVIEW_CONTROL_H_