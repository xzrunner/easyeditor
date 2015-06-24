// Original coders: Trent Polack (trent@voxelsoft.com)

#ifndef _DRAG2D_TIMER_H_
#define _DRAG2D_TIMER_H_

namespace d2d
{

class Timer
{
public:
	bool Init();

	void Update();

	float GetTime() const;

	float GetFps() const { return m_fFPS; }

private:
	//performance timer variables
	__int64       m_i64PerformanceTimerStart;
	__int64       m_i64PerformanceTimerElapsed;

	//multimedia timer variables
	unsigned long m_ulMMTimerElapsed;
	unsigned long m_ulMMTimerStart;

	//general timer variables
	__int64		  m_i64Frequency;
	float		  m_fResolution;
	bool		  m_bPerformanceTimer;

	//FPS variables
	float m_fTime1;
	float m_fTime2;
	float m_fDiffTime;
	float m_fFPS;
	int	m_iFramesElapsed;

}; // Timer

}

#endif // _DRAG2D_TIMER_H_