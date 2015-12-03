#include "EditDialog.h"
#include "StagePanel.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Icon.h"
#include "IconType.h"
#include "FileIO.h"

#include "EditRectOP.h"
#include "EditQuadOP.h"
#include "EditChangedRectOP.h"
#include "EditChangedSectorOP.h"

namespace eicon
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx,
					   d2d::ISprite* edited, const d2d::MultiSpritesImpl* sprite_impl)
 	: wxDialog(parent, wxID_ANY, "Edit Shape", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_stage(NULL)
{
	InitLayout(glctx, edited, sprite_impl);
	InitEditOP(edited);

	m_stage->SetTitleStatus(true);	
}

void EditDialog::InitLayout(wxGLContext* glctx, d2d::ISprite* edited, 
							const d2d::MultiSpritesImpl* sprite_impl)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_stage = new StagePanel(this, this, glctx, edited, sprite_impl);
	sizer->Add(m_stage, 1, wxEXPAND);

	SetSizer(sizer);
}

void EditDialog::InitEditOP(d2d::ISprite* edited)
{
	d2d::AbstractEditOP* op = NULL;

	const Icon* icon = static_cast<Sprite*>(edited)->GetSymbol().GetIcon();
	IconType type = get_icon_type(icon->GetIconDesc());
	switch (type)
	{
	case IT_RECT:
		op = new EditRectOP(m_stage);
		break;
	case IT_QUAD:
		op = new EditQuadOP(m_stage);
		break;
	case IT_CHANGED_RECT:
		op = new EditChangedRectOP(m_stage);
		break;
	case IT_CHANGED_SECTOR:
		op = new EditChangedSectorOP(m_stage);
		break;
	}

	m_stage->SetEditOP(op);
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

	Symbol& symbol = m_stage->GetSymbol();
	const std::string& filepath = symbol.GetFilepath();

	d2d::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		FileIO::StoreToFile(filepath.c_str(), symbol.GetIcon());
		symbol.RefreshThumbnail(filepath);
		d2d::SpriteFactory::Instance()->updateBoundings(symbol);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		symbol.LoadFromFile(filepath);
		Destroy();
	}
}

}