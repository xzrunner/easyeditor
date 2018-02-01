#include "SprPropColMonitor.h"
#include "EditPanelImpl.h"
#include "StageCanvas.h"
#include "panel_msg.h"

#include <sprite2/Sprite.h>

namespace ee
{

SprPropColMonitor::SprPropColMonitor(const SprPtr& spr, ColType type)
	: m_spr(spr)
	, m_type(type)
{
}

pt2::Color SprPropColMonitor::GetColor() const
{
	switch (m_type)
	{
	case CT_MUL:
		return m_spr->GetColorCommon().mul;
	case CT_ADD:
		return m_spr->GetColorCommon().add;
	case CT_RMAP:
		return m_spr->GetColorMap().rmap;
	case CT_GMAP:
		return m_spr->GetColorMap().gmap;
	case CT_BMAP:
		return m_spr->GetColorMap().bmap;
	default:
		return pt2::Color();
	}
}

void SprPropColMonitor::OnColorChanged()
{
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void SprPropColMonitor::OnColorChanged(const pt2::Color& col)
{
	auto& col_common = m_spr->GetColorCommon();
	auto& col_map = m_spr->GetColorMap();
	switch (m_type)
	{
	case CT_MUL:
		m_spr->SetColorCommon(pt2::RenderColorCommon(col, col_common.add));
		break;
	case CT_ADD:
		m_spr->SetColorCommon(pt2::RenderColorCommon(col_common.mul, col));
		break;
	case CT_RMAP:
		m_spr->SetColorMap(pt2::RenderColorMap(col, col_map.gmap, col_map.bmap));
		break;
	case CT_GMAP:
		m_spr->SetColorMap(pt2::RenderColorMap(col_map.rmap, col, col_map.bmap));
		break;
	case CT_BMAP:
		m_spr->SetColorMap(pt2::RenderColorMap(col_map.rmap, col_map.gmap, col));
		break;
	}

	OnColorChanged();
}

}