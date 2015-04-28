#include "PlayControl.h"

#include <wx/log.h>

namespace d2d
{

PlayControl::PlayControl(float dt) 
	: m_curr_frame(1), m_last(-1)
{
	m_dt = dt * CLOCKS_PER_SEC;
}

PlayControl::~PlayControl() 
{}

bool PlayControl::update() 
{
	bool refresh = false;

	if (m_last == -1) {
		m_last = clock();
	} else {
		clock_t curr = clock();
		int dt = curr - m_last;

//		wxLogDebug(_T("update dt = %d"), dt);

		while (dt > m_dt) {
//			wxLogDebug(_T("++ frame"));

			dt -= m_dt;
			++m_curr_frame;
			refresh = true;
		}
		m_last = curr - dt;			
	}

	return refresh;
}

// bool PlayControl::update() 
// {
// 	bool refresh = false;
// 
// 	if (m_last == -1) {
// 		m_init = m_last = clock();
// 	} else {
// 		clock_t curr = clock();
// 		float during = (float)(curr - m_init) / CLOCKS_PER_SEC;
// 		m_curr_frame = during / m_dt * CLOCKS_PER_SEC;
// 	}
// 
// 	return refresh;
// }

void PlayControl::reset() 
{
	m_curr_frame = 1;
}

void PlayControl::decrease() 
{
	--m_curr_frame;
}

int PlayControl::frame() const 
{
	return m_curr_frame;
}

}