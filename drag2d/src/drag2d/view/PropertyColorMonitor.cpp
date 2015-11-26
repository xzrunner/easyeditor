#include "PropertyColorMonitor.h"

#include "common/Color.h"
#include "view/EditPanelImpl.h"
#include "view/IStageCanvas.h"
#include "message/SetCanvasDirtySJ.h"

namespace d2d
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