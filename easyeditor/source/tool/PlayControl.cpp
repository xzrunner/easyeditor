#include "PlayControl.h"

namespace ee
{

PlayControl::PlayControl(float dt) 
	: m_curr_frame(1), m_last(-1)
{
	SetDt(dt);
}

PlayControl::~PlayControl() 
{}

bool PlayControl::Update() 
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

void PlayControl::Reset() 
{
	m_curr_frame = 1;
}

void PlayControl::Decrease() 
{
	--m_curr_frame;
}

int PlayControl::Frame() const 
{
	return m_curr_frame;
}

void PlayControl::SetDt(float dt) 
{ 
	m_dt = static_cast<int>(dt * CLOCKS_PER_SEC);
}

}