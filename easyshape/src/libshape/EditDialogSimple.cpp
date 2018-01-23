#include "EditDialogSimple.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Sprite.h"

#include "EditRectOP.h"
#include "EditCircleOP.h"
#include "EditPolylineOP.h"
#include "DrawPolygonOP.h"
#include "EditBezierOP.h"

#include <ee/panel_msg.h>
#include <ee/shape_msg.h>
#include <ee/SpritePool.h>
#include <ee/SelectShapesOP.h>
#include <ee/ConfirmDialog.h>

namespace eshape
{

BEGIN_EVENT_TABLE(EditDialogSimple, wxDialog)
	EVT_CLOSE(EditDialogSimple::OnCloseEvent)
END_EVENT_TABLE()

EditDialogSimple::EditDialogSimple(wxWindow* parent, wxGLContext* glctx,
								   ee::SprPtr edited, const ee::MultiSpritesImpl* sprite_impl)
 	: wxDialog(parent, wxID_ANY, "Edit Shape", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_stage(nullptr)
	, m_capture(5)
{
	InitLayout(glctx, edited, sprite_impl);
	InitEditOP(edited);

	ee::SetWndDirtySJ::Instance()->SetDirty();
}

EditDialogSimple::~EditDialogSimple()
{
}

void EditDialogSimple::InitLayout(wxGLContext* glctx, const ee::SprPtr& edited, 
								  const ee::MultiSpritesImpl* sprite_impl)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_stage = new StagePanel(this, this, glctx, edited, sprite_impl);
	sizer->Add(m_stage, 1, wxEXPAND);

	SetSizer(sizer);
}

void EditDialogSimple::InitEditOP(const ee::SprPtr& edited)
{
	std::shared_ptr<ee::EditOP> op = nullptr;

	ShapeType type = std::dynamic_pointer_cast<Symbol>(edited->GetSymbol())->GetShapeType();
	switch (type)
	{
	case ST_RECT:
		op = std::make_shared<EditRectOP>(m_stage, m_stage->GetStageImpl(), m_stage, nullptr, &m_capture);
		break;
	case ST_CIRCLE:
		op = std::make_shared<EditCircleOP>(m_stage, m_stage->GetStageImpl(), m_stage, nullptr, &m_capture);
		break;
	case ST_CHAIN: case ST_POLYGON:
		op = std::make_shared<EditPolylineOP<DrawPolygonOP, ee::SelectShapesOP>>(
			m_stage, m_stage->GetStageImpl(), m_stage, nullptr, new ee::OneFloatValueStatic(5), nullptr); 
		break;
	case ST_BEZIER:
		op = std::make_shared<EditBezierOP>(m_stage, m_stage->GetStageImpl(), m_stage, nullptr, &m_capture);
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

	ee::Symbol& sym = const_cast<ee::Symbol&>(m_stage->GetSymbol());
	const std::string& filepath = sym.GetFilepath();

	ee::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		static_cast<Symbol&>(sym).StoreToFile(filepath.c_str());
		sym.RefreshThumbnail(filepath);
		ee::SpritePool::Instance()->UpdateBoundings(sym);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		sym.LoadFromFile(filepath);
		Destroy();
	}
}

}