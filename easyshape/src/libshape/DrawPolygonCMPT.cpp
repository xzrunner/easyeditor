#include "DrawPolygonCMPT.h"
#include "EditPolylineWithCopyNodeOP.h"
#include "PolygonShape.h"

#include <wx/colordlg.h>

namespace libshape
{

DrawPolygonCMPT::DrawPolygonCMPT(wxWindow* parent, const wxString& name,
								 d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
								 d2d::PropertySettingPanel* propertyPanel/*, wxColourData& colorData*/)
	: d2d::OneFloatValueCMPT(parent, name, editPanel, "node capture", 5, 30, 10)
	, m_shapesImpl(shapesImpl)
	, m_color(*wxBLACK)
//	, m_colorData(colorData)
{
	m_editOP = NULL;

	addChild(new d2d::UniversalCMPT(this, wxT("直接画线"), editPanel, 
		new EditPolylineOP<DrawPolygonEdgeOP, d2d::SelectShapesOP>(editPanel, shapesImpl, propertyPanel, this, this)));
	addChild(new d2d::UniversalCMPT(this, wxT("选节点画线"), editPanel, 
		new EditPolylineWithCopyNodeOP<DrawPolygonEdgeOP>(editPanel, shapesImpl, propertyPanel)));
}

void DrawPolygonCMPT::updateControlValue()
{
	bool empty;
	m_shapesImpl->getShapeSelection()->Traverse(d2d::EmptyVerifyVisitor(empty));
	m_btnTrigger->Enable(!empty);
}

wxSizer* DrawPolygonCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(d2d::OneFloatValueCMPT::initLayout());

	sizer->AddSpacer(20);

	sizer->Add(initChildrenLayout());

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
		{
//			wxColourDialog dialog(this/*, &m_colorData*/);
//			dialog.SetTitle(wxT("选择颜色"));
//			if (dialog.ShowModal() == wxID_OK)
//			{
//				m_color = dialog.GetColourData().GetColour();
////				m_colorData = dialog.GetColourData();
//			}

			// todo trans between wxColor and d2d::Colorf
			d2d::Colorf col;
			col.r = m_color.Red() / 255.0f;
			col.g = m_color.Green() / 255.0f;
			col.b = m_color.Blue() / 255.0f;
			d2d::HSLColorSettingDlg dlg(m_editPanel, NULL, col);
			if (dlg.ShowModal()) {
				col = dlg.GetColor();
				m_color.Set(col.r * 255, col.g * 255, col.b * 255);
				m_editPanel->ResetViewport();
			}
		}
		break;
	case 1:
		{
			wxFileDialog dlg(this, wxT("选择纹理"), wxEmptyString, wxEmptyString, wxT("*.png;*.jpg"), wxFD_OPEN);
			if (dlg.ShowModal() == wxID_OK)
			{
				m_filePath = dlg.GetPath();
				m_bitmap.loadFromFile(m_filePath);
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
	m_shapesImpl->getShapeSelection()->Traverse(d2d::FetchAllVisitor<PolygonShape>(polys));

	for (size_t i = 0, n = polys.size(); i < n; ++i)
	{
		PolygonShape* poly = polys[i];
		switch (m_fillingTypeChoice->GetSelection()) 
		{
		case 0:
			poly->SetMaterialColor(d2d::Colorf(m_color.Red() / 255.0f, m_color.Green() / 255.0f, m_color.Blue() / 255.0f));
			break;
		case 1:
			poly->SetMaterialTexture(static_cast<d2d::ImageSymbol*>(d2d::SymbolMgr::Instance()->fetchSymbol(m_filePath)));
			break;
		}
		m_editPanel->Refresh();
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
		if (const wxBitmap* bmp = m_bitmap.getBitmap()) {
			m_btnReview->SetBitmap(*bmp);
		}
		break;
	}
}

}