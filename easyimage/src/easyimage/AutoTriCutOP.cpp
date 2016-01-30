#include "AutoTriCutOP.h"

namespace eimage
{

AutoTriCutOP::AutoTriCutOP(wxWindow* wnd, d2d::EditPanelImpl* stage)
	: d2d::ZoomViewOP(wnd, stage, true)
{
}

bool AutoTriCutOP::OnActive()
{
	if (d2d::ZoomViewOP::OnActive()) {
		return true;
	}

	d2d::SettingData& setting = d2d::Config::Instance()->GetSettings();
	setting.open_image_edge_clip = false;
	setting.linear_filter = false;

	return false;
}

bool AutoTriCutOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) {
		return true;
	}

// 	d2d::PrimitiveDraw::DrawPolyline(m_raw_bound_line, d2d::LIGHT_RED, false);
// 	d2d::PrimitiveDraw::DrawPoints(m_raw_bound_points, d2d::LIGHT_RED, 5);

 	d2d::PrimitiveDraw::DrawPolyline(m_raw_bound_line_merged, d2d::LIGHT_RED, false);
 	d2d::PrimitiveDraw::DrawPolyline(m_fine_bound_line, d2d::LIGHT_BLUE, true);

	return false;
}

}