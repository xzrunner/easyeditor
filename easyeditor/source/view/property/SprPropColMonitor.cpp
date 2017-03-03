#include "SprPropColMonitor.h"
#include "EditPanelImpl.h"
#include "StageCanvas.h"
#include "panel_msg.h"

#include <sprite2/S2_Sprite.h>
#include <sprite2/RenderColor.h>

namespace ee
{

SprPropColMonitor::SprPropColMonitor(s2::Sprite* spr, ColType type)
	: m_spr(spr)
	, m_type(type)
{
}

s2::Color SprPropColMonitor::GetColor() const
{
	switch (m_type)
	{
	case CT_MUL:
		return m_spr->GetColor().GetMul();
	case CT_ADD:
		return m_spr->GetColor().GetAdd();
	case CT_RMAP:
		return m_spr->GetColor().GetRMap();
	case CT_GMAP:
		return m_spr->GetColor().GetGMap();
	case CT_BMAP:
		return m_spr->GetColor().GetBMap();
	default:
		return s2::Color();
	}
}

void SprPropColMonitor::OnColorChanged()
{
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void SprPropColMonitor::OnColorChanged(const s2::Color& col)
{
	s2::RenderColor rc = m_spr->GetColor();
	switch (m_type)
	{
	case CT_MUL:
		rc.SetMul(col);
		break;
	case CT_ADD:
		rc.SetAdd(col);
		break;
	case CT_RMAP:
		rc.SetRMap(col);
		break;
	case CT_GMAP:
		rc.SetGMap(col);
		break;
	case CT_BMAP:
		rc.SetBMap(col);
		break;
	}
	m_spr->SetColor(rc);

	OnColorChanged();
}

}