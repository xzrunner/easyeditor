#include "EditDialogSimple.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Sprite.h"

#include "EditRectOP.h"
#include "EditCircleOP.h"
#include "EditPolylineOP.h"
#include "DrawPolygonOP.h"
#include "EditBezierOP.h"

namespace eshape
{

BEGIN_EVENT_TABLE(EditDialogSimple, wxDialog)
	EVT_CLOSE(EditDialogSimple::OnCloseEvent)
END_EVENT_TABLE()

EditDialogSimple::EditDialogSimple(wxWindow* parent, wxGLContext* glctx,
								   ee::Sprite* edited, const ee::MultiSpritesImpl* sprite_impl)
 	: wxDialog(parent, wxID_ANY, "Edit Shape", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_stage(NULL)
	, m_capture(5)
{
	InitLayout(glctx, edited, sprite_impl);
	InitEditOP(edited);

	ee::SetWndDirtySJ::Instance()->SetDirty();
}

EditDialogSimple::~EditDialogSimple()
{
}

void EditDialogSimple::InitLayout(wxGLContext* glctx, ee::Sprite* edited, 
								  const ee::MultiSpritesImpl* sprite_impl)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_stage = new StagePanel(this, this, glctx, edited, sprite_impl);
	sizer->Add(m_stage, 1, wxEXPAND);

	SetSizer(sizer);
}

void EditDialogSimple::InitEditOP(ee::Sprite* edited)
{
	ee::EditOP* op = NULL;

	ShapeType type = static_cast<Sprite*>(edited)->GetSymbol().GetShapeType();
	switch (type)
	{
	case ST_RECT:
		op = new EditRectOP(m_stage, m_stage->GetStageImpl(), m_stage, NULL, &m_capture);
		break;
	case ST_CIRCLE:
		op = new EditCircleOP(m_stage, m_stage->GetStageImpl(), m_stage, NULL, &m_capture);
		break;
	case ST_CHAIN: case ST_POLYGON:
		op = new EditPolylineOP<DrawPolygonOP, 
			ee::SelectShapesOP>(m_stage, m_stage->GetStageImpl(), m_stage, NULL, new ee::OneFloatValueStatic(5), NULL); 
		break;
	case ST_BEZIER:
		op = new EditBezierOP(m_stage, m_stage->GetStageImpl(), m_stage, NULL, &m_capture);
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

	ee::Symbol& symbol = const_cast<ee::Symbol&>(m_stage->GetSymbol());
	const std::string& filepath = symbol.GetFilepath();

	ee::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		static_cast<Symbol&>(symbol).StoreToFile(filepath.c_str());
		symbol.RefreshThumbnail(filepath);
		ee::SpriteFactory::Instance()->UpdateBoundings(symbol);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		symbol.LoadFromFile(filepath);
		Destroy();
	}
}

}