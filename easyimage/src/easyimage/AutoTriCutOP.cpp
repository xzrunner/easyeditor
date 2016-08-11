#include "AutoTriCutOP.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/color_config.h>

#include <sprite2/S2_RVG.h>

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

// 	s2::RVG::Polyline(m_raw_bound_line, ee::LIGHT_RED, false);
// 	s2::RVG::Points(m_raw_bound_points, ee::LIGHT_RED, 5);

	s2::RVG::SetColor(ee::LIGHT_RED);
 	s2::RVG::Polyline(m_raw_bound_line_merged, false);
	s2::RVG::SetColor(ee::LIGHT_BLUE);
 	s2::RVG::Polyline(m_fine_bound_line, true);

	return false;
}

}