#include "PropertyColorMonitor.h"
#include "EditPanelImpl.h"
#include "StageCanvas.h"
#include "panel_msg.h"

namespace ee
{

PropertyColorListener::PropertyColorListener(s2::Color* col)
	: m_col(col)
{
}

s2::Color PropertyColorListener::GetColor() const
{
	return *m_col;	
}

void PropertyColorListener::OnColorChanged()
{
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void PropertyColorListener::OnColorChanged(const s2::Color& col)
{
	*m_col = col;
	OnColorChanged();
}

}