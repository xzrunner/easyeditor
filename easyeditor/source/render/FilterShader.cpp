#include "FilterShader.h"
#include "FilterModes.h"

#include <sl_filter.h>

#include <assert.h>

namespace ee
{

FilterShader::FilterShader()
{
}

FilterShader::~FilterShader()
{
}

void FilterShader::Load()
{
	sl_filter_load();
}

void FilterShader::Unload()
{
	sl_filter_unload();
}

void FilterShader::Bind()
{
	sl_filter_bind();
}

void FilterShader::Unbind()
{
	sl_filter_unbind();
}

void FilterShader::SetProjection(int width, int height)
{
	sl_filter_projection(width, height);
}

void FilterShader::Commit()
{
	sl_filter_commit();
}

void FilterShader::Reset()
{
}

void FilterShader::SetModelView(const Vector& offset, float scale)
{
	sl_filter_modelview(offset.x, offset.y, scale, scale);
}

void FilterShader::SetMode(const std::string& str)
{
	FilterMode mode = FilterModes::Instance()->GetIDFromNameEN(str);

	SL_FILTER_MODE sl_mode = SLFM_MAX_COUNT;
	switch (mode)
	{
	case FM_EDGE_DETECTION:
		sl_mode = SLFM_EDGE_DETECTION;
		break;
	case FM_RELIEF:
		sl_mode = SLFM_RELIEF;
		break;
	case FM_OUTLINE:
		sl_mode = SLFM_OUTLINE;
		break;
	case FM_BLUR:
		sl_mode = SLFM_BLUR;
		break;
	case FM_GRAY:
		sl_mode = SLFM_GRAY;
		break;
	}
	assert(sl_mode != SLFM_MAX_COUNT);

	sl_filter_set_mode(sl_mode);
}

void FilterShader::Draw(const Vector vertices[4], const Vector texcoords[4], int texid)
{
	sl_filter_draw(&vertices[0].x, &texcoords[0].x, texid);
}

}