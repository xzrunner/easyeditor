#pragma once

#include <painting2/FilterMode.h>

#include <memory>

namespace pt2
{

class RenderFilter
{
public:
	RenderFilter() : m_mode(pt2::FM_NULL) {}
	RenderFilter(const RenderFilter& filter)
		: m_mode(filter.m_mode)
	{}
	RenderFilter(pt2::FilterMode mode) 
		: m_mode(mode) 
	{}
	virtual ~RenderFilter() {}

	virtual bool operator == (const RenderFilter& rf) const {
		return GetMode() == rf.GetMode();
	}

	virtual RenderFilter* Clone() const = 0;

	virtual size_t Size() const = 0;

	pt2::FilterMode GetMode() const { return m_mode; }	

	static void Deleter(RenderFilter* filter);

private:
	pt2::FilterMode m_mode;

}; // RenderFilter

using RenderFilterPtr = std::unique_ptr<RenderFilter, decltype(&RenderFilter::Deleter)>;

}