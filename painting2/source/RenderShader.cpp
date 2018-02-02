#include "painting2/RenderShader.h"
#include "painting2/RenderFilter.h"
#include "painting2/FilterFactory.h"

namespace pt2
{

RenderShader::RenderShader()
#ifdef PT2_FILTER_FULL
	: m_filter(nullptr, RenderFilter::Deleter)
#else
	: m_filter(pt2::FM_NULL)
#endif // PT2_FILTER_FULL
	, m_blend(pt2::BM_NULL)
	, m_fast_blend(pt2::FBM_NULL)
	, m_downsample(1)
{
}

RenderShader::RenderShader(const RenderShader& rs)
#ifdef PT2_FILTER_FULL
	: m_filter(nullptr, RenderFilter::Deleter)
#else
	: m_filter(pt2::FM_NULL)
#endif // PT2_FILTER_FULL
{
#ifdef PT2_FILTER_FULL
	if (rs.m_filter) {
		m_filter.reset(rs.m_filter->Clone());
	}
#else
	m_filter     = rs.m_filter;
#endif // PT2_FILTER_FULL
	m_blend      = rs.m_blend;
	m_fast_blend = rs.m_fast_blend;
	m_downsample = rs.m_downsample;
}

RenderShader& RenderShader::operator = (const RenderShader& rs)
{
#ifdef PT2_FILTER_FULL
	if (m_filter != rs.m_filter) 
	{
		if (rs.m_filter) {
			m_filter.reset(rs.m_filter->Clone());
		} else {
			m_filter.reset();
		}
	}
#else
	m_filter     = rs.m_filter;
#endif // PT2_FILTER_FULL
	m_blend      = rs.m_blend;
	m_fast_blend = rs.m_fast_blend;
	m_downsample = rs.m_downsample;
	return *this;
}

bool RenderShader::operator == (const RenderShader& rs) const
{
	return
#ifdef PT2_FILTER_FULL
		((!m_filter && !rs.m_filter) || (m_filter && rs.m_filter && *m_filter == *rs.m_filter)) &&
#else
		m_filter == rs.m_filter &&
#endif // PT2_FILTER_FULL
		m_blend == rs.m_blend &&
		m_fast_blend == rs.m_fast_blend &&
		m_downsample == rs.m_downsample;
}

bool RenderShader::operator != (const RenderShader& rs) const
{
	return !(this->operator == (rs));
}

void RenderShader::Reset()
{
#ifdef PT2_FILTER_FULL
	m_filter.reset();
#else
	m_filter = pt2::FM_NULL;
#endif // PT2_FILTER_FULL
	m_blend = pt2::BM_NULL;
	m_fast_blend = pt2::FBM_NULL;
	m_downsample = 1;
}

RenderShader RenderShader::operator * (const RenderShader& rs) const
{
#ifdef PT2_FILTER_FULL
	return Multiply(rs.GetFilter().get(), rs.GetBlend(), rs.GetFastBlend(), rs.GetDownsample());
#else
	return Multiply(rs.GetFilter(), rs.GetBlend(), rs.GetFastBlend(), rs.GetDownsample());
#endif // PT2_FILTER_FULL
}

#ifdef PT2_FILTER_FULL
RenderShader RenderShader::Multiply(const RenderFilter* filter,
	                                pt2::BlendMode blend, 
	                                pt2::FastBlendMode fast_blend, 
	                                float downsample) const
{
	RenderShader ret;

	if (blend != pt2::BM_NULL) {
		ret.m_blend = blend;
	}
	else {
		ret.m_blend = m_blend;
	}

	if (fast_blend != pt2::FBM_NULL) {
		ret.m_fast_blend = fast_blend;
	}
	else {
		ret.m_fast_blend = m_fast_blend;
	}

	if (filter && filter->GetMode() != pt2::FM_NULL) {
		ret.m_filter.reset(filter->Clone());
	}
	else if (m_filter) {
		ret.m_filter.reset(m_filter->Clone());
	}

	ret.m_downsample = downsample * m_downsample;

	return ret;
}
#else
RenderShader RenderShader::Multiply(pt2::FilterMode filter,
	                                pt2::BlendMode blend, 
	                                pt2::FastBlendMode fast_blend, 
	                                float downsample) const
{
	RenderShader ret;

	if (blend != pt2::BM_NULL) {
		ret.m_blend = blend;
	} else {
		ret.m_blend = m_blend;
	}

	if (fast_blend != pt2::FBM_NULL) {
		ret.m_fast_blend = fast_blend;
	} else {
		ret.m_fast_blend = m_fast_blend;
	}

	if (filter != pt2::FM_NULL) {
		ret.m_filter = filter;
	} else {
		ret.m_filter = m_filter;
	}

	ret.m_downsample = downsample * m_downsample;

	return ret;
}
#endif // PT2_FILTER_FULL

void RenderShader::SetFilter(pt2::FilterMode mode)
{
#ifdef PT2_FILTER_FULL
	if (m_filter && m_filter->GetMode() == mode) {
		return;
	}

	m_filter = FilterFactory::Instance()->Create(mode);
#else
	m_filter = mode;
#endif // PT2_FILTER_FULL
}

#ifdef PT2_FILTER_FULL
void RenderShader::SetFilter(const RenderFilter* filter)
{
	if (m_filter.get() == filter) {
		return;
	}

	if (filter) {
		m_filter.reset(filter->Clone());
	} else {
		m_filter.reset();
	}
}
#endif // PT2_FILTER_FULL

}