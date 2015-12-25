#include "LevelEditDlg.h"

#include "dataset/LevelCfg.h"
#include "dataset/DynamicWidget.h"

namespace lr
{

LevelEditDlg::LevelEditDlg(wxWindow* parent, d2d::ISprite* spr)
	: CfgEditDlg(parent, "Level", spr)
{	
	InitType(spr->tag);
	InitLayout();
}

std::string LevelEditDlg::ToString()
{
	std::string ret = "type=" + m_type + ";";
	for (int i = 0, n = m_widgets.size(); i < n; ++i) {
		DynamicWidget* widget = m_widgets[i];
		m_info.SetValue(widget->GetKey(), widget->GetValue(), widget->GetValue() == DEFAULT_VAL);
	}
	ret += m_info.ToString();
	return ret;
}

void LevelEditDlg::InitType(const std::string& tag)
{
	std::vector<std::string> tags;
	d2d::StringTools::Split(tag, ";", tags);

	std::string type_tag;
	for (int i = 0, n = tags.size(); i < n; ++i) {
		if (tags[i].find("type") != std::string::npos) {
			type_tag = tags[i];
			break;
		}
	}

	if (!type_tag.empty() && 
		type_tag.find('=') != std::string::npos) {
		m_type = type_tag.substr(type_tag.find_first_of('=') + 1);
	}
}

void LevelEditDlg::InitLayout()
{
	if (m_type.empty()) {
		return;
	}

	int col;
	if (!LevelCfg::Instance()->QueryLayout(m_type, col, m_widgets)) {
		return;
	}

	CfgEditDlg::InitLayout(col, m_widgets);
}

}