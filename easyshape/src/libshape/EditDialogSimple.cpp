#include "EditDialogSimple.h"
#include "StagePanel.h"
#include "Symbol.h"

#include "BezierShape.h"
#include "ChainShape.h"
#include "CircleShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

#include "EditRectOP.h"
#include "EditCircleOP.h"
#include "EditPolylineOP.h"
#include "DrawPolygonEdgeOP.h"
#include "EditBezierOP.h"

namespace libshape
{

BEGIN_EVENT_TABLE(EditDialogSimple, wxDialog)
	EVT_CLOSE(EditDialogSimple::OnClose)
END_EVENT_TABLE()

EditDialogSimple::EditDialogSimple(wxWindow* parent, Symbol* symbol)
 	: wxDialog(parent, wxID_ANY, "Edit Shape", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_stage(NULL)
	, m_capture(5)
{
	InitLayout(symbol);
	InitEditOP(symbol);
}

EditDialogSimple::~EditDialogSimple()
{
}

void EditDialogSimple::InitLayout(Symbol* symbol)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_stage = new StagePanel(this, this, symbol);
	sizer->Add(m_stage, 1, wxEXPAND);

	SetSizer(sizer);
}

void EditDialogSimple::InitEditOP(Symbol* symbol)
{
	d2d::AbstractEditOP* op = NULL;

	ShapeType type = symbol->GetType();
	switch (type)
	{
	case e_rect:
		op = new EditRectOP(m_stage, m_stage, NULL, &m_capture);
		break;
	case e_circle:
		op = new EditCircleOP(m_stage, m_stage, NULL, &m_capture);
		break;
	case e_chain: case e_polygon:
		op = new EditPolylineOP<DrawPolygonEdgeOP, 
			d2d::SelectShapesOP>(m_stage, m_stage, NULL, NULL, 5); 
		break;
	case e_bezier:
		op = new EditBezierOP(m_stage, m_stage, NULL, &m_capture);
		break;
	}

	m_stage->setEditOP(op);
}

void EditDialogSimple::OnClose(wxCloseEvent& event)
{
	if (!m_stage->isDirty())
	{
		Destroy();
		return;
	}

	d2d::ISymbol& symbol = const_cast<d2d::ISymbol&>(m_stage->GetSymbol());
	const wxString& filepath = symbol.getFilepath();

	d2d::ExitDlg dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_OK)
	{
		static_cast<Symbol&>(symbol).StoreToFile(filepath);
		symbol.RefreshThumbnail(filepath);
		d2d::SpriteFactory::Instance()->updateBoundings(symbol);
	}
	else if (val == wxID_CANCEL)
	{
		symbol.loadFromFile(filepath);
	}

	Destroy();
}

}