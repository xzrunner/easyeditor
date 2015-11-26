#include "RightPopupMenu.h"
#include "StagePanel.h"
#include "typedef.h"

#include "dataset/Layer.h"

#include <easycomplex.h>

namespace lr
{

RightPopupMenu::RightPopupMenu(StagePanel* stage)
	: m_stage(stage)
	, m_sprite(NULL)
{
}

void RightPopupMenu::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	if (m_stage->GetSpriteSelection()->Size() != 1) {
		return;
	}

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	m_stage->GetSpriteSelection()->Traverse(d2d::PointQueryVisitor(pos, &m_sprite));
	CreateShapeMenu(menu);
	CreateAnimMenu(menu);
	CreateLayerTagMenu(menu);
}

void RightPopupMenu::OnRightPopupMenu(int id)
{
	if (id == MENU_OPEN_WITH_SHAPE_ID) {
		HandleShapeMenu(id);
	} else if (id == MENU_ROTATE_LEFT_ID || id == MENU_ROTATE_RIGHT_ID || id - MENU_COLOR_START_ID < m_anim_files.size()) {
		HandleAnimMenu(id);
	} else if (id >= MENU_LAYER_CLEAR_TAG_ID && id <= MENU_TOP_LAYER_TAG_ID) {
		HandleLayerTagMenu(id);
	}
}

void RightPopupMenu::CreateShapeMenu(wxMenu& menu)
{
	menu.AppendSeparator();
	m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, MENU_OPEN_WITH_SHAPE_ID);
	menu.Append(MENU_OPEN_WITH_SHAPE_ID, "Open With EasyShape");
}

void RightPopupMenu::CreateAnimMenu(wxMenu& menu)
{
	std::string filepath = m_sprite->GetSymbol().GetFilepath();
	if (!CharacterFileName::IsValidFilepath(filepath)) {
		return;
	}

	// rotate
	menu.AppendSeparator();
	m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, MENU_ROTATE_LEFT_ID);
	menu.Append(MENU_ROTATE_LEFT_ID, "Rotate Left");
	m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, MENU_ROTATE_RIGHT_ID);
	menu.Append(MENU_ROTATE_RIGHT_ID, "Rotate Right");

	// color

	// 	Layer* layer = m_stage->GetCurrLayer();
	// 	if (layer->GetName() != "单位") {
	// 		return;
	// 	}

	FetchCandidateAnimFiles(filepath);
	if (m_anim_files.empty()) {
		return;
	}

	menu.AppendSeparator();

	for (int i = 0, n = m_anim_files.size(); i < n; ++i) {
		int id = MENU_COLOR_START_ID+i;
		m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, id);
		menu.Append(id, m_anim_files[i].GetField(CharacterFileName::FT_COLOR));
	}
}

void RightPopupMenu::CreateLayerTagMenu(wxMenu& menu)
{
	if (!m_sprite) {
		return;
	}

	menu.AppendSeparator();

	const std::string& tag = m_sprite->tag;
	if (tag.find(std::string(COVER_LAYER_TAG)) == std::string::npos) {
		m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, MENU_COVER_LAYER_TAG_ID);
		menu.Append(MENU_COVER_LAYER_TAG_ID, "遮挡层");		
	} 
	if (tag.find(std::string(TOP_LAYER_TAG)) == std::string::npos) {
		m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, MENU_TOP_LAYER_TAG_ID);
		menu.Append(MENU_TOP_LAYER_TAG_ID, "顶层");	
	}
	if (tag.find("layer=") != std::string::npos) {
		m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, MENU_LAYER_CLEAR_TAG_ID);
		menu.Append(MENU_LAYER_CLEAR_TAG_ID, "清除层信息");	
	}
}

void RightPopupMenu::HandleShapeMenu(int id)
{
	std::vector<d2d::ISprite*> selected;
	m_stage->GetSpriteSelection()->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	if (!selected.empty()) {
		std::string cmd = "easyshape_new.exe " + selected[0]->GetSymbol().GetFilepath();
		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);		
	}
}

void RightPopupMenu::HandleAnimMenu(int id)
{
	if (id == MENU_ROTATE_LEFT_ID || id == MENU_ROTATE_RIGHT_ID)
	{
		std::string filepath = m_sprite->GetSymbol().GetFilepath();
		assert(CharacterFileName::IsValidFilepath(filepath));
		CharacterFileName name(filepath);
		int dir = 1 + (name.GetField(CharacterFileName::FT_DIRECTION)[0] - '1');
		if (m_sprite->GetMirrorX()) {
			dir = 10 - dir;
		}

		bool rot_left = id == MENU_ROTATE_LEFT_ID;
		if (rot_left) { --dir; }
		else { ++dir; }
		if (dir == 0) { dir = 8; }
		else if (dir == 9) { dir = 1; }

		d2d::ISymbol* symbol = m_stage->GetCharaDirs()->GetSymbolByDir(filepath, dir);
		static_cast<ecomplex::Sprite*>(m_sprite)->SetSymbol(symbol);


		if (dir >= 1 && dir <= 5) {
			m_sprite->SetMirror(false, false);
		} else {
			m_sprite->SetMirror(true, false);
		}
	}
	else if (id - MENU_COLOR_START_ID < m_anim_files.size())
	{
		const CharacterFileName& item = m_anim_files[id - MENU_COLOR_START_ID];

		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(item.GetFilepath());
		static_cast<ecomplex::Sprite*>(m_sprite)->SetSymbol(symbol);
	}
}

void RightPopupMenu::HandleLayerTagMenu(int id)
{
	std::string& tag = m_sprite->tag;

	size_t p_begin = tag.find("layer=");
	if (p_begin != std::string::npos) {
		size_t p_end = tag.find(";", p_begin) + 1;
		tag.erase(tag.begin() + p_begin, tag.begin() + p_end);
	}

	if (id == MENU_COVER_LAYER_TAG_ID) {
		m_sprite->tag += std::string(COVER_LAYER_TAG) + ";";
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	} else if (id == MENU_TOP_LAYER_TAG_ID) {
		m_sprite->tag += std::string(TOP_LAYER_TAG) + ";";
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void RightPopupMenu::FetchCandidateAnimFiles(const std::string& filepath)
{
	m_anim_files.clear();

	CharacterFileName name(filepath);

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files, d2d::FileNameParser::e_complex);

	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();

		if (!CharacterFileName::IsValidFilepath(filepath)) {
			continue;
		}

		CharacterFileName name_other(filepath);
		if (name.FieldSameExcept(name_other, CharacterFileName::FT_COLOR)) {
			m_anim_files.push_back(name_other);
		}
	}
}

}