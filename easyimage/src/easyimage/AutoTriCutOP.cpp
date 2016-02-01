#include "AutoTriCutOP.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/PrimitiveDraw.h>
#include <ee/style_config.h>

namespace eimage
{

AutoTriCutOP::AutoTriCutOP(wxWindow* wnd, ee::EditPanelImpl* stage)
	: ee::ZoomViewOP(wnd, stage, true)
{
}

bool AutoTriCutOP::OnActive()
{
	if (ee::ZoomViewOP::OnActive()) {
		return true;
	}

	ee::SettingData& setting = ee::Config::Instance()->GetSettings();
	setting.open_image_edge_clip = false;
	setting.linear_filter = false;

	return false;
}

bool AutoTriCutOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) {
		return true;
	}

// 	ee::PrimitiveDraw::DrawPolyline(m_raw_bound_line, ee::LIGHT_RED, false);
// 	ee::PrimitiveDraw::DrawPoints(m_raw_bound_points, ee::LIGHT_RED, 5);

 	ee::PrimitiveDraw::DrawPolyline(m_raw_bound_line_merged, ee::LIGHT_RED, false);
 	ee::PrimitiveDraw::DrawPolyline(m_fine_bound_line, ee::LIGHT_BLUE, true);

	return false;
}

}