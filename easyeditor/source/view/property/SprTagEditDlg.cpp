#include "SprTagEditDlg.h"
#include "SprTagCfg.h"
#include "Sprite.h"
#include "DynamicWidget.h"

namespace ee
{

SprTagEditDlg::SprTagEditDlg(wxWindow* parent, Sprite* spr)
	: CfgEditDlg(parent, "Tag", spr)
{	
	m_info.LoadFromString(spr->GetTag());

	InitDefault();
	InitLayout();
}

std::string SprTagEditDlg::ToString()
{
	SprTagCfg* cfg = SprTagCfg::Instance();
	const std::vector<DynamicWidget*>& widgets = cfg->GetWidgets();
	for (int i = 0, n = widgets.size(); i < n; ++i) {
		const DynamicWidget* widget = widgets[i];
		m_info.SetValue(widget->GetKey(), widget->GetValue(), widget->GetValue() == DynamicWidget::DEFAULT_VAL);
	}	 
	return m_info.ToString();
}

void SprTagEditDlg::InitDefault()
{
// 	// patrol
// 	ChoiceWidget* patrol = new ChoiceWidget;
// 	patrol->SpecialLoad("Ñ²Âß", "patrol", m_path_names, m_info.QueryValue("patrol"));
// 	m_special_widgets.push_back(patrol);
}

void SprTagEditDlg::InitLayout()
{
	SprTagCfg* cfg = SprTagCfg::Instance();
	CfgEditDlg::InitLayout(1, cfg->GetWidgets());
}

}