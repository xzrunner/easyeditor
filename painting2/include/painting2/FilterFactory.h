#pragma once

#include "painting2/RenderFilter.h"

#include <cu/cu_macro.h>
#include <painting2/FilterMode.h>

#include <memory>

namespace pt2
{

class FilterFactory
{
public:
	RenderFilterPtr Create(pt2::FilterMode mode);

	RenderFilter* GetTemp(pt2::FilterMode mode) {
		return m_temps[mode];
	}

private:
	static const int MAX_COUNT = 100;

private:
	RenderFilter* m_temps[MAX_COUNT];

private:
	CU_SINGLETON_DECLARATION(FilterFactory)

}; // FilterFactory

}