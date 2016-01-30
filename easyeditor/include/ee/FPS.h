#ifndef _EASYEDITOR_FPS_STAT_H_
#define _EASYEDITOR_FPS_STAT_H_

#include <string>

#include <time.h>

namespace ee
{

class Screen;

class FPS
{
public:
	FPS(float interval);

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

#endif // _EASYEDITOR_FPS_STAT_H_