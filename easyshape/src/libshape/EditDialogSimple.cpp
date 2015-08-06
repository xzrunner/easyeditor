#include "EditDialogSimple.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Sprite.h"

#include "EditRectOP.h"
#include "EditCircleOP.h"
#include "EditPolylineOP.h"
#include "DrawPolygonOP.h"
#include "EditBezierOP.h"

namespace libshape
{

BEGIN_EVENT_TABLE(EditDialogSimple, wxDialog)
	EVT_CLOSE(EditDialogSimple::OnCloseEvent)
END_EVENT_TABLE()

EditDialogSimple::EditDialogSimple(wxWindow* parent, d2d::ISprite* edited,
								   const d2d::MultiSpritesImpl* sprite_impl)
 	: wxDialog(parent, wxID_ANY, "Edit Shape", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_stage(NULL)
	, m_capture(5)
{
	InitLayout(edited, sprite_impl);
	InitEditOP(edited);

	m_stage->SetTitleStatus(true);
}

EditDialogSimple::~EditDialogSimple()
{
}

void EditDialogSimple::InitLayout(d2d::ISprite* edited, const d2d::MultiSpritesImpl* sprite_impl)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_stage = new StagePanel(this, this, edited, sprite_impl);
	sizer->Add(m_stage, 1, wxEXPAND);

	SetSizer(sizer);
}

void EditDialogSimple::InitEditOP(d2d::ISprite* edited)
{
	d2d::AbstractEditOP* op = NULL;

	ShapeType type = static_cast<Sprite*>(edited)->GetSymbol().GetShapeType();
	switch (type)
	{
	case ST_RECT:
		op = new EditRectOP(m_stage, m_stage->GetStageImpl(), m_stage, NULL, NULL, &m_capture);
		break;
	case ST_CIRCLE:
		op = new EditCircleOP(m_stage, m_stage->GetStageImpl(), m_stage, NULL, NULL, &m_capture);
		break;
	case ST_CHAIN: case ST_POLYGON:
		op = new EditPolylineOP<DrawPolygonOP, 
			d2d::SelectShapesOP>(m_stage, m_stage->GetStageImpl(), m_stage, NULL, NULL, new d2d::OneFloatValueStatic(5), NULL); 
		break;
	case ST_BEZIER:
		op = new EditBezierOP(m_stage, m_stage->GetStageImpl(), m_stage, NULL, NULL, &m_capture);
		break;
	}

	m_stage->SetEditOP(op);
}

void EditDialogSimple::OnCloseEvent(wxCloseEvent& event)
{
	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

	d2d::ISymbol& symbol = const_cast<d2d::ISymbol&>(m_stage->GetSymbol());
	const std::string& filepath = symbol.GetFilepath();

	d2d::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		static_cast<Symbol&>(symbol).StoreToFile(filepath.c_str());
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