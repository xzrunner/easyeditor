#pragma once

#include "painting2/RenderFilter.h"

#include <cu/cu_stl.h>
#include <memmgr/Allocator.h>

namespace pt2
{

class RFHeatHaze : public RenderFilter
{
public:
	RFHeatHaze() 
		: RenderFilter(pt2::FM_HEAT_HAZE) 
		, m_distortion_factor(0.02f)
		, m_rise_factor(0.2f)
	{}

	virtual bool operator == (const RenderFilter& rf) const override {
		const RFHeatHaze& rf_hh = static_cast<const RFHeatHaze&>(rf);
		return RenderFilter::operator == (rf) &&
			   m_filepath == rf_hh.m_filepath &&
			   m_distortion_factor == rf_hh.m_distortion_factor &&
			   m_rise_factor == rf_hh.m_rise_factor;
	}

	virtual RenderFilter* Clone() const override {
		return new (mm::AllocHelper::Allocate(Size())) RFHeatHaze(*this);
	}

	virtual size_t Size() const override { return sizeof(RFHeatHaze); }

	const CU_STR& GetFilepath() const { return m_filepath; }
	void SetFilepath(const CU_STR& filepath) { m_filepath = filepath; }

	void GetFactor(float& distortion, float& rise) const {
		distortion = m_distortion_factor;
		rise = m_rise_factor;
	}
	void SetFactor(float distortion, float rise) {
		m_distortion_factor = distortion;
		m_rise_factor = rise;
	}

private:
	CU_STR m_filepath;		// distortion map texture

	float m_distortion_factor, m_rise_factor;

}; // RFHeatHaze

}