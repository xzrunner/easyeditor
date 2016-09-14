#include "RenderShader.h"
#include "RenderFilter.h"
#include "FilterFactory.h"

namespace s2
{

RenderShader::RenderShader()
	: filter(NULL)
	, blend(BM_NULL)
	, fast_blend(FBM_NULL)
{}

RenderShader::RenderShader(const RenderShader& rs)
	: filter(NULL)
	, blend(rs.blend)
	, fast_blend(rs.fast_blend)
{
	if (rs.filter) {
		filter = new RenderFilter(rs.filter->GetMode());
	}
}

RenderShader& RenderShader::operator = (const RenderShader& rs)
{
	if (rs.filter) {
		filter = new RenderFilter(rs.filter->GetMode());
	} else {
		filter = NULL;
	}
	blend = rs.blend;
	fast_blend = rs.fast_blend;
	return *this;
}

RenderShader::~RenderShader()
{
	// todo 
//	delete filter;
}

RenderShader RenderShader::operator * (const RenderShader& rs) const
{
	RenderShader ret;

	if (rs.blend != BM_NULL) {
		ret.blend = rs.blend;
	} else {
		ret.blend = blend;
	}

	if (rs.fast_blend != FBM_NULL) {
		ret.fast_blend = rs.fast_blend;
	} else {
		ret.fast_blend = fast_blend;
	}

	if (rs.filter && rs.filter->GetMode()!= FM_NULL) {
		ret.filter = rs.filter;
	} else {
		ret.filter = filter;
	}

	return ret;
}

void RenderShader::SetFilter(s2::FilterMode mode)
{
	if (filter && filter->GetMode() == mode) {
		return;
	}

	RenderFilter* _filter = FilterFactory::Instance()->Create(mode);
	if (filter) {
		delete filter;
	}
	filter = _filter;
}

}