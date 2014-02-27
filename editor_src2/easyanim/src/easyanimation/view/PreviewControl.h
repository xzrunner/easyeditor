#ifndef _EASYANIM_PREVIEW_CONTROL_H_
#define _EASYANIM_PREVIEW_CONTROL_H_

namespace eanim
{

class PreviewControl
{
public:
	PreviewControl() : m_curr_frame(1) {}

	void update() {
		++m_curr_frame;
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

}; // PreviewControl

}

#endif // _EASYANIM_PREVIEW_CONTROL_H_