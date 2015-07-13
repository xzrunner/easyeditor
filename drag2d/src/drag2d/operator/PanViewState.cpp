#include "PanViewState.h"

#include "view/Camera.h"
#include "view/EditPanel.h"

namespace d2d
{

PanViewState::PanViewState(EditPanel* stage)
	: m_stage(stage)
{
	m_last_pos.setInvalid();
}

void PanViewState::Bind()
{
	m_stage->SetCursor(wxCURSOR_HAND);
}

void PanViewState::UnBind()
{
	m_stage->SetCursor(wxCURSOR_ARROW);
}

bool PanViewState::OnMousePress(int x, int y)
{
	m_last_pos.set(x, y);
	return false;
}

bool PanViewState::OnMouseRelease(int x, int y)
{
	m_last_pos.setInvalid();
	return false;
}

bool PanViewState::OnMouseDrag(int x, int y)
{
	if (m_last_pos.isValid()) {
		Vector offset(m_last_pos.x - x, m_last_pos.y - y);
		offset.y = -offset.y;
		m_stage->GetCamera()->Translate(offset);

		m_last_pos.set(x, y);

		m_stage->SetCanvasDirty();
	}
	return false;
}

}