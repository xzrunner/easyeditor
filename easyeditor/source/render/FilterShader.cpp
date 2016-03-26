#include "FilterShader.h"
#include "FilterModes.h"

#include <sl_filter.h>

#include <assert.h>

namespace ee
{

FilterShader::FilterShader()
	: m_update(false)
{
}

FilterShader::~FilterShader()
{
}

void FilterShader::Load()
{
	sl_filter_load();

	sl_filter_set_heat_haze_tex(4);
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
	if (m_update) {
		Update(0.03f);
	}
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
	case FM_HEAT_HAZE:
		sl_mode = SLFM_HEAT_HAZE;
		break;
	}
	assert(sl_mode != SLFM_MAX_COUNT);

	sl_filter_set_mode(sl_mode);
}

void FilterShader::Update(float dt)
{
	sl_filter_update(dt);
}

void FilterShader::Draw(const Vector vertices[4], const Vector texcoords[4], int texid)
{
	m_update = true;

	sl_filter_draw(&vertices[0].x, &texcoords[0].x, texid);
}

}