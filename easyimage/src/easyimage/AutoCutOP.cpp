#include "AutoCutOP.h"

namespace eimage
{

AutoCutOP::AutoCutOP(d2d::EditPanel* stage)
	: d2d::ZoomViewOP(stage, true)
{
}

bool AutoCutOP::onActive()
{
	if (d2d::ZoomViewOP::onActive()) {
		return true;
	}

	d2d::SettingData& setting = d2d::Config::Instance()->GetSettings();
	setting.open_image_edge_clip = false;
	setting.linear_filter = false;

	return false;
}

bool AutoCutOP::onDraw() const
{
	if (d2d::ZoomViewOP::onDraw()) {
		return true;
	}

	d2d::PrimitiveDraw::drawPolyline(m_raw_bound_line, d2d::LIGHT_RED, false);
//	d2d::PrimitiveDraw::drawPoints(m_raw_bound_points, d2d::LIGHT_RED, 5);
	d2d::PrimitiveDraw::drawPolyline(m_fine_bound_line, d2d::LIGHT_BLUE, false);

	return false;
}

}