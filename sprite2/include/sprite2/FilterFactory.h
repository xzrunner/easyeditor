#ifndef _SPRITE2_FILTER_FACTORY_H_
#define _SPRITE2_FILTER_FACTORY_H_

#include "Singleton.h"
#include "FilterMode.h"

namespace s2
{

class RenderFilter;

class FilterFactory
{
public:
	RenderFilter* Create(FilterMode mode);

	RenderFilter* GetTemp(FilterMode mode) {
		return m_temps[mode];
	}

private:
	static const int MAX_COUNT = 100;

private:
	RenderFilter* m_temps[MAX_COUNT];

private:
	SINGLETON_DECLARATION(FilterFactory)

}; // FilterFactory

}

#endif // _SPRITE2_FILTER_FACTORY_H_