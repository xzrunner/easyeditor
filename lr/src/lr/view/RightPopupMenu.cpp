#include "RightPopupMenu.h"
#include "StagePanel.h"

#include "dataset/Layer.h"

#include <easycomplex.h>

namespace lr
{

RightPopupMenu::RightPopupMenu(StagePanel* stage)
	: m_stage(stage)
	, m_sprite(NULL)
{
}

void RightPopupMenu::SetRightPopupMenu(wxMenu& menu, d2d::ISprite* spr)
{
// 	Layer* layer = m_stage->GetCurrLayer();
// 	if (layer->GetName() != "µ¥Î»") {
// 		return;
// 	}

	std::string filepath = spr->getSymbol().getFilepath();
	FetchCandidateItems(filepath);
	if (m_items.empty()) {
		return;
	}

	m_sprite = spr;

	menu.AppendSeparator();

	for (int i = 0, n = m_items.size(); i < n; ++i) {
		int id = MENU_START_ID+i;
		m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, id);
		menu.Append(id, m_items[i].color);
	}
}

void RightPopupMenu::OnRightPopupMenu(int id)
{
	const AnimFileName& item = m_items[id - MENU_START_ID];

	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(item.filepath);
	static_cast<ecomplex::Sprite*>(m_sprite)->ChangeSymbol(static_cast<ecomplex::Symbol*>(symbol));

//	int zz = 0;
}

void RightPopupMenu::FetchCandidateItems(const std::string& filepath)
{
	m_items.clear();

	if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
		return;
	}

	AnimFileName name(filepath);
	if (!name.IsValid()) {
		return;
	}

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();

		AnimFileName name_other(filepath);
		if (name_other.IsValid() && name.SameExceptColor(name_other)) {
			m_items.push_back(name_other);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// struct RightPopupMenu::AnimFileName
//////////////////////////////////////////////////////////////////////////

RightPopupMenu::AnimFileName::
AnimFileName(const std::string& filepath)
	: filepath(filepath)
{
	std::string filename = d2d::FilenameTools::getFilename(filepath);

	while (true)
	{
		std::string str = filename;
		int pos = str.find_first_of("_");
		if (pos == std::string::npos) { break; }
		character = str.substr(0, pos);

		str = str.substr(pos+1);
		pos = str.find_first_of("_");
		if (pos == std::string::npos) { break; }
		action = str.substr(0, pos);

		str = str.substr(pos+1);
		pos = str.find_first_of("_");
		if (pos == std::string::npos) { break; }
		direction = str.substr(0, pos);

		str = str.substr(pos+1);
		pos = str.find_first_of("_");
		if (pos == std::string::npos) { break; }
		color = str.substr(0, pos);

		postfix = str.substr(pos+1);

		break;
	}
}

bool RightPopupMenu::AnimFileName::
SameExceptColor(const RightPopupMenu::AnimFileName& name) const
{
	return character == name.character
		&& action == name.action
		&& direction == name.direction
		&& color != name.color
		&& postfix == name.postfix;
}

}