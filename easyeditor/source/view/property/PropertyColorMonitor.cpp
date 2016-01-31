#include "PropertyColorMonitor.h"
#include "Color.h"
#include "EditPanelImpl.h"
#include "StageCanvas.h"
#include "panel_msg.h"

namespace ee
{

PropertyColorListener::PropertyColorListener(Colorf* col)
	: m_col(col)
{
}

Colorf PropertyColorListener::GetColor() const
{
	return *m_col;	
}

void PropertyColorListener::OnColorChanged()
{
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void PropertyColorListener::OnColorChanged(const Colorf& col)
{
	*m_col = col;
	OnColorChanged();
}

}