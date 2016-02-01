#include "DrawPolygonCMPT.h"
#include "EditPolylineWithCopyNodeOP.h"
#include "PolygonShape.h"
#include "DrawPencilPolygonCMPT.h"

namespace eshape
{

DrawPolygonCMPT::DrawPolygonCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd,
								 ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapesImpl,
								 ee::PropertySettingPanel* property)
	: ee::OneFloatValueCMPT(parent, name, stage, "node capture", 5, 30, 10)
	, m_stage_wnd(stage_wnd)
	, m_shapesImpl(shapesImpl)
	, m_color(*wxBLACK)
{
	m_editOP = NULL;
	// draw polygon with pen, node capture
	{
		ee::OneFloatValueCMPT* cmpt = new ee::OneFloatValueCMPT(this, "pen", stage, "node capture", 5, 30, 10);
		ee::EditOP* op = new EditPolylineOP<DrawPolygonOP, ee::SelectShapesOP>
			(stage_wnd, stage, shapesImpl, property, cmpt, /*cmpt*/this);
		cmpt->SetEditOP(op);
		addChild(cmpt);
	}
	// draw polygon with pencil, simplify threshold
	{
		addChild(new DrawPencilPolygonCMPT(this, wxT("pencil"), stage_wnd, stage, shapesImpl));
	}
}

void DrawPolygonCMPT::updateControlValue()
{
	bool empty;
	m_shapesImpl->GetShapeSelection()->Traverse(ee::EmptyVerifyVisitor(empty));
	m_btnTrigger->Enable(!empty);
}

wxSizer* DrawPolygonCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(ee::OneFloatValueCMPT::InitLayout());

	sizer->AddSpacer(20);

	sizer->Add(InitChildrenLayout());

	sizer->AddSpacer(20);

	sizer->Add(initPreviewButtonPanel());

	sizer->AddSpacer(10);

	m_btnTrigger = new wxButton(this, wxID_ANY, wxT("设置"));
	Connect(m_btnTrigger->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DrawPolygonCMPT::onTriggerFillingColor));
	sizer->Add(m_btnTrigger);
	m_btnTrigger->Enable(false);

	return sizer;
}

wxSizer* DrawPolygonCMPT::initPreviewButtonPanel()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxArrayString choices;
	choices.Add(wxT("颜色"));
	choices.Add(wxT("纹理"));
	m_fillingTypeChoice = new wxRadioBox(this, wxID_ANY, wxT("填充方式"), wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
	Connect(m_fillingTypeChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(DrawPolygonCMPT::onChangeFillingType));
	sizer->Add(m_fillingTypeChoice);

	sizer->AddSpacer(10);

	m_btnReview = new wxButton(this, wxID_ANY);	
	Connect(m_btnReview->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DrawPolygonCMPT::onSetColor));
	sizer->Add(m_btnReview);
	fillingButton();

	return sizer;
}

void DrawPolygonCMPT::onSetColor(wxCommandEvent& event)
{
	switch (m_fillingTypeChoice->GetSelection()) 
	{
	case 0:
// 		{
// 			// todo trans between wxColor and ee::Colorf
// 			ee::Colorf col;
// 			col.r = m_color.Red() / 255.0f;
// 			col.g = m_color.Green() / 255.0f;
// 			col.b = m_color.Blue() / 255.0f;
// 			ee::HSLColorSettingDlg dlg(m_stage_wnd, NULL, col);
// 			if (dlg.ShowModal() == wxID_OK) {
// 				col = dlg.GetColor();
// 				m_color.Set(col.r * 255, col.g * 255, col.b * 255);
// 			}
// 		}
		{
			// todo trans between wxColor and ee::Colorf
			ee::Colorf col;
			col.r = m_color.Red() / 255.0f;
			col.g = m_color.Green() / 255.0f;
			col.b = m_color.Blue() / 255.0f;
			ee::RGBColorSettingDlg dlg(m_stage_wnd, NULL, col);
			if (dlg.ShowModal() == wxID_OK) {
				col = dlg.GetColor();
				m_color.Set(col.r * 255, col.g * 255, col.b * 255);
			}
		}
		break;
	case 1:
		{
			wxFileDialog dlg(this, wxT("选择纹理"), wxEmptyString, wxEmptyString, wxT("*.png;*.jpg"), wxFD_OPEN);
			if (dlg.ShowModal() == wxID_OK)
			{
				m_filePath = dlg.GetPath();
				m_bitmap.LoadFromFile(m_filePath);
			}
		}
		break;
	}
	fillingButton();
}

void DrawPolygonCMPT::onChangeFillingType(wxCommandEvent& event)
{
	fillingButton();
}

void DrawPolygonCMPT::onTriggerFillingColor(wxCommandEvent& event)
{
	std::vector<PolygonShape*> polys;
	m_shapesImpl->GetShapeSelection()->Traverse(ee::FetchAllVisitor<PolygonShape>(polys));

	for (size_t i = 0, n = polys.size(); i < n; ++i)
	{
		PolygonShape* poly = polys[i];
		switch (m_fillingTypeChoice->GetSelection()) 
		{
		case 0:
			poly->SetMaterialColor(ee::Colorf(m_color.Red() / 255.0f, m_color.Green() / 255.0f, m_color.Blue() / 255.0f));
			break;
		case 1:
			poly->SetMaterialTexture(static_cast<ee::ImageSymbol*>(ee::SymbolMgr::Instance()->FetchSymbol(m_filePath)));
			break;
		}
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void DrawPolygonCMPT::fillingButton()
{
	switch (m_fillingTypeChoice->GetSelection()) 
	{
	case 0:
		{
			wxImage img(m_btnReview->GetSize().GetWidth(), m_btnReview->GetSize().GetHeight());
			img.SetRGB(wxRect(m_btnReview->GetSize()), m_color.Red(), m_color.Green(), m_color.Blue());
			wxBitmap bitmap(img);
			m_btnReview->SetBitmap(bitmap);
		}
		break;
	case 1:
		if (const wxBitmap* bmp = m_bitmap.GetLargeBmp()) {
			m_btnReview->SetBitmap(*bmp);
		}
		break;
	}
}

}