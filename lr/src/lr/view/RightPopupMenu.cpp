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
	// open with shape
	menu.AppendSeparator();
	m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, MENU_OPEN_WITH_SHAPE_ID);
	menu.Append(MENU_OPEN_WITH_SHAPE_ID, "Open With EasyShape");

	// color

// 	Layer* layer = m_stage->GetCurrLayer();
// 	if (layer->GetName() != "µ¥Î»") {
// 		return;
// 	}

	std::string filepath = spr->getSymbol().getFilepath();
	FetchCandidateAnimFiles(filepath);
	if (m_anim_files.empty()) {
		return;
	}

	m_sprite = spr;

	menu.AppendSeparator();

	for (int i = 0, n = m_anim_files.size(); i < n; ++i) {
		int id = MENU_COLOR_START_ID+i;
		m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, id);
		menu.Append(id, m_anim_files[i].color);
	}
}

void RightPopupMenu::OnRightPopupMenu(int id)
{
	if (id == MENU_OPEN_WITH_SHAPE_ID)
	{
		std::vector<d2d::ISprite*> selected;
		m_stage->getSpriteSelection()->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
		if (!selected.empty()) {
			std::string cmd = "easyshape_new.exe " + selected[0]->getSymbol().getFilepath();
			WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);		
		}
	}
	else
	{
		const AnimFileName& item = m_anim_files[id - MENU_COLOR_START_ID];

		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(item.filepath);
		static_cast<ecomplex::Sprite*>(m_sprite)->setSymbol(symbol);
	}
}

void RightPopupMenu::FetchCandidateAnimFiles(const std::string& filepath)
{
	m_anim_files.clear();

	if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
		return;
	}

	std::string filename = d2d::FilenameTools::getFilename(filepath);
	if (!AnimFileName::CanAccept(filename)) {
		return;
	}

	AnimFileName name(filepath);

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files, d2d::FileNameParser::e_complex);

	for (int i = 0, n = files.size(); i < n; ++i)
	{
// 		if (!d2d::FileNameParser::isType(files[i], d2d::FileNameParser::e_complex) ||
// 			!AnimFileName::CanAccept(d2d::FilenameTools::getFilename(files[i]).ToStdString())) {
// 			continue;
// 		}

		if (!AnimFileName::CanAccept(d2d::FilenameTools::getFilename(files[i]).ToStdString())) {
			continue;
		}

		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();

		AnimFileName name_other(filepath);
		if (name.SameExceptColor(name_other)) {
			m_anim_files.push_back(name_other);
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

	int last_pos = 0;

	int pos = filename.find_first_of("_", last_pos);
	character = filename.substr(last_pos, pos);
	last_pos = pos + 1;

	pos = filename.find_first_of("_", last_pos);
	action = filename.substr(last_pos, pos - last_pos);
	last_pos = pos + 1;

	pos = filename.find_first_of("_", last_pos);
	direction = filename.substr(last_pos, pos - last_pos);
	last_pos = pos + 1;

	pos = filename.find_first_of("_", last_pos);
	color = filename.substr(last_pos, pos - last_pos);
	last_pos = pos + 1;

	postfix = filename.substr(pos+1);
}

bool RightPopupMenu::AnimFileName::CanAccept(const std::string& filepath)
{
	int count = 0;
	for (int i = 0, n = filepath.size(); i < n; ++i) {
		if (filepath[i] == '_') {
			++count;
		}
	}
	return count == 4;
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