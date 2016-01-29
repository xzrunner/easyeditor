#include "CfgEditDlg.h"

#include "dataset/DynamicWidget.h"

namespace lr
{

CfgEditDlg::CfgEditDlg(wxWindow* parent, const char* name, d2d::ISprite* spr)
	: d2d::BaseDialog(parent, name)
	, m_spr(spr)
{
}

void CfgEditDlg::InitLayout(int col, const std::vector<DynamicWidget*>& widgets)
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	wxSizer* main_sizer = new wxBoxSizer(wxHORIZONTAL);

	col = std::max(2, col);

	std::vector<wxSizer*> col_sizers;
	for (int i = 0; i < col; ++i) {
		col_sizers.push_back(new wxBoxSizer(wxVERTICAL));
	}

	int count = std::ceil((float)(widgets.size()) / col);

	for (int i = 0, n = widgets.size(); i < n; ++i) {
		int idx = i / count;
		wxSizer* sz = col_sizers[idx];

		DynamicWidget* dw = widgets[i];
		dw->SetTag(m_info.QueryTag(dw->GetKey()));
		dw->InitLayout(this, sz, m_info);
	}

	for (int i = 0; i < col; ++i) {
		main_sizer->Add(col_sizers[i]);
		main_sizer->AddSpacer(20);
	}
	top_sizer->Add(main_sizer);

	top_sizer->AddSpacer(20);
	top_sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxCENTER);
	top_sizer->AddSpacer(20);

	SetSizer(top_sizer);
	top_sizer->Fit(this);

	wxWindow* parent = m_parent->GetParent()->GetParent()->GetParent();
	wxPoint p_pos = parent->GetPosition();
	wxSize p_size = parent->GetSize();
	wxSize c_size = top_sizer->GetSize();
	wxPoint c_pos;
	c_pos.x = p_pos.x + (p_size.GetWidth() - c_size.GetWidth()) * 0.5f;
	c_pos.y = p_pos.y + (p_size.GetHeight() - c_size.GetHeight()) * 0.5f;
	SetPosition(c_pos);
}

}