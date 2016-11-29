#include "PropColMonitor.h"
#include "EditPanelImpl.h"
#include "StageCanvas.h"
#include "panel_msg.h"

namespace ee
{

PropColMonitor::PropColMonitor(s2::Color* col)
	: m_col(col)
{
}

s2::Color PropColMonitor::GetColor() const
{
	return *m_col;	
}

void PropColMonitor::OnColorChanged()
{
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void PropColMonitor::OnColorChanged(const s2::Color& col)
{
	*m_col = col;
	OnColorChanged();
}

}