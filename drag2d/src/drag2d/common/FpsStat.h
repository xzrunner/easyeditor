#ifndef _DRAG2D_FPS_STAT_H_
#define _DRAG2D_FPS_STAT_H_

#include <time.h>
#include <string>

namespace d2d
{

class Screen;

class FpsStat
{
public:
	FpsStat(float interval);

	void Begin();
	void End();

	void DrawTime(const Screen& scr) const; 
	void DrawFPS(const Screen& scr) const;

private:
	void Draw(const Screen& scr, const std::string& str) const;

private:
	int m_interval;

	clock_t m_last_time;

	int m_tot_cost;
	int m_count;

	std::string m_time, m_fps;

}; // FpsStat

}

#endif // _DRAG2D_FPS_STAT_H_