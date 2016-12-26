#ifndef _SHADERLAB_STATISTICS_H_
#define _SHADERLAB_STATISTICS_H_

#include <CU_Singleton.h>

namespace sl
{

class Statistics
{
public:
	void AddDrawCall() { ++m_drawcall; }
	void AddVertices(int count) { m_vertices += count; }
//	void AddOverdrawArea(int area) { m_overdraw_area += area; }

	int GetDrawCall() const { return m_drawcall; }
	int GetVertices() const { return m_vertices; }
//	int GetOverdrawArea() const { return m_overdraw_area; }

	void Reset();

private:
	int m_drawcall;
	int m_vertices;
//	int m_overdraw_area;

	SINGLETON_DECLARATION(Statistics);

}; // Statistics

}

#endif // _SHADERLAB_STATISTICS_H_