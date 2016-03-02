#include "AutoTriCutOP.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/EE_RVG.h>
#include <ee/color_config.h>

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

// 	ee::RVG::Polyline(m_raw_bound_line, ee::LIGHT_RED, false);
// 	ee::RVG::Points(m_raw_bound_points, ee::LIGHT_RED, 5);

	ee::RVG::Color(ee::LIGHT_RED);
 	ee::RVG::Polyline(m_raw_bound_line_merged, false);
	ee::RVG::Color(ee::LIGHT_BLUE);
 	ee::RVG::Polyline(m_fine_bound_line, true);

	return false;
}

}