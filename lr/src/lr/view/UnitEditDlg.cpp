#include "UnitEditDlg.h"

#include "dataset/UnitWidget.h"

namespace lr
{

UnitEditDlg::UnitEditDlg(wxWindow* parent, d2d::ISprite* spr,
						 const std::vector<std::string>& path_names)
	: BaseDialog(parent, wxT("Unit"))
	, m_spr(spr)
	, m_info(spr->tag)
	, m_path_names(path_names)
{	
	InitDefault();
	InitLayout();
}

UnitEditDlg::~UnitEditDlg()
{
//	for_each(m_special_widgets.begin(), m_special_widgets.end(), DeletePointerFunctor<UnitWidget>());
}

std::string UnitEditDlg::ToString()
{
	UnitCfg* cfg = UnitCfg::Instance();
	for (int i = 0, n = cfg->widgets.size(); i < n; ++i) {
		UnitWidget* widget = cfg->widgets[i];
		m_info.SetValue(widget->GetKey(), widget->GetValue(), widget->GetValue() == DEFAULT_VAL);
	}
// 	for (int i = 0, n = m_special_widgets.size(); i < n; ++i) {
// 		UnitWidget* widget = m_special_widgets[i];
// 		m_info.SetValue(widget->GetKey(), widget->GetValue());
// 	}
	 
	return m_info.ToString();
}

void UnitEditDlg::InitDefault()
{
// 	// patrol
// 	UnitChoiceWidget* patrol = new UnitChoiceWidget;
// 	patrol->SpecialLoad("Ñ²Âß", "patrol", m_path_names, m_info.QueryValue("patrol"));
// 	m_special_widgets.push_back(patrol);
}

void UnitEditDlg::InitLayout()
{
	UnitCfg* cfg = UnitCfg::Instance();

	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	wxSizer* main_sizer = new wxBoxSizer(wxHORIZONTAL);

	int col = std::max(2, cfg->col);

	std::vector<wxSizer*> col_sizers;
	for (int i = 0; i < col; ++i) {
		col_sizers.push_back(new wxBoxSizer(wxVERTICAL));
	}

	int count = std::ceil((float)(cfg->widgets.size()) / col);

	for (int i = 0, n = cfg->widgets.size(); i < n; ++i) {
		int idx = i / count;
		wxSizer* sz = col_sizers[idx];
		cfg->widgets[i]->InitLayout(this, sz, m_info);
	}

	for (int i = 0; i < col; ++i) {
		main_sizer->Add(col_sizers[i]);
		main_sizer->AddSpacer(20);
	}
	top_sizer->Add(main_sizer);

	top_sizer->AddSpacer(20);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxButton(this, wxID_OK), 0, wxALL, 5);
		sz->Add(new wxButton(this, wxID_CANCEL), 0, wxALL, 5);
		top_sizer->Add(sz, 0, wxCENTER);
	}
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