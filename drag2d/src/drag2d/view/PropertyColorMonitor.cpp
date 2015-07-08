#include "PropertyColorMonitor.h"

#include "common/Color.h"
#include "view/EditPanel.h"

namespace d2d
{

PropertyColorListener::PropertyColorListener(EditPanel* stage, Colorf* col)
	: m_stage(stage)
	, m_col(col)
{
}

Colorf PropertyColorListener::GetColor() const
{
	return *m_col;	
}

void PropertyColorListener::OnColorChanged()
{
	m_stage->RefreshStage();
}

void PropertyColorListener::OnColorChanged(const Colorf& col)
{
	*m_col = col;
	OnColorChanged();
}

}