#include "StagePanel.h"
#include "StageCanvas.h"

namespace emap
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library,
					   ee::PropertySettingPanel* property)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, ee::ShapesPanelImpl(GetStageImpl())
{
	m_paste_op = new ee::PasteSymbolOP(this, GetStageImpl(), this, library);
	m_arrange_op = new ee::ArrangeSpriteOP<SelectSpritesOP>(this, GetStageImpl(), this, property);

	SetEditOP(m_paste_op);
	SetCanvas(new StageCanvas(this));
}

StagePanel::~StagePanel()
{
}

void StagePanel::OnMouseHook(wxMouseEvent& event)
{
	if (event.RightDown()) {
		ChangeEditOP();
	}	
}

void StagePanel::ChangeEditOP()
{
	if (!GetEditOP()->IsEmpty()) {
		return;
	}

	if (GetEditOP() == m_arrange_op) {
		SetEditOP(m_paste_op);
	} else {
		SetEditOP(m_arrange_op);
	}
}

}