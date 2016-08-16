#include "RightPopupMenu.h"
#include "StagePanel.h"
#include "typedef.h"

#include "dataset/Layer.h"
#include "view/LibraryPage.h"

#include <ee/SpriteSelection.h>
#include <ee/PointQueryVisitor.h>
#include <ee/SpriteSelection.h>
#include <ee/Layer.h>
#include <ee/FetchAllVisitor.h>
#include <ee/panel_msg.h>
#include <ee/SymbolMgr.h>
#include <ee/FileHelper.h>

#include <easycomplex.h>

namespace lr
{

RightPopupMenu::RightPopupMenu(ee::LibraryPanel* library, StagePanel* stage)
	: m_library(library)
	, m_stage(stage)
	, m_sprite(NULL)
{
}

void RightPopupMenu::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	ee::SpriteSelection* selection = m_stage->GetSpriteSelection();
	if (selection->Size() == 1) {
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		selection->Traverse(ee::PointQueryVisitor(pos, &m_sprite));
		CreateShapeMenu(menu);
		CreateAnimMenu(menu);
	}
	CreateLayerTagMenu(menu);
	CreateLayerMoveMenu(menu);
}

void RightPopupMenu::OnRightPopupMenu(int id)
{
	if (id == MENU_OPEN_WITH_SHAPE_ID) {
		HandleShapeMenu(id);
	} else if (id == MENU_ROTATE_LEFT_ID || id == MENU_ROTATE_RIGHT_ID || id - MENU_COLOR_START_ID < m_anim_files.size()) {
		HandleAnimMenu(id);
	} else if (id >= MENU_LAYER_CLEAR_TAG_ID && id <= MENU_TOP_LAYER_TAG_ID) {
		HandleLayerTagMenu(id);
	} else if (id >= MENU_MOVE_TO_LAYER_BEGIN_ID && id < MENU_MOVE_TO_LAYER_END_ID) {
		HandleMoveToLayerMenu(id);
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
	std::string filepath = m_sprite->GetSymbol()->GetFilepath();
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
	menu.AppendSeparator();

	if (m_sprite && m_stage->GetSpriteSelection()->Size() == 1) 
	{
		const std::string& tag = m_sprite->GetTag();
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
	else 
	{
		m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, MENU_COVER_LAYER_TAG_ID);
		menu.Append(MENU_COVER_LAYER_TAG_ID, "遮挡层");

		m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, MENU_TOP_LAYER_TAG_ID);
		menu.Append(MENU_TOP_LAYER_TAG_ID, "顶层");

		m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, MENU_LAYER_CLEAR_TAG_ID);
		menu.Append(MENU_LAYER_CLEAR_TAG_ID, "清除层信息");	
	}
}

void RightPopupMenu::CreateLayerMoveMenu(wxMenu& menu)
{
	Layer* layer = static_cast<LibraryPage*>(m_library->GetCurrPage())->GetLayer();
	const std::vector<ee::Layer*>& layers = layer->GetLayerMgr()->GetAllLayers();
	wxMenu* sub_menu = new wxMenu;
	for (int i = 0, n = layers.size(); i < n; ++i) {
		int id = MENU_MOVE_TO_LAYER_BEGIN_ID + i;
		sub_menu->Append(id, layers[i]->name);
		m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnRightPopupMenu, m_stage, id);
	}
	menu.AppendSubMenu(sub_menu, "移动到");
}

void RightPopupMenu::HandleShapeMenu(int id)
{
	std::vector<ee::Sprite*> selected;
	m_stage->GetSpriteSelection()->Traverse(ee::FetchAllVisitor<ee::Sprite>(selected));
	if (!selected.empty()) {
		std::string cmd = "easyshape_new.exe " + selected[0]->GetSymbol()->GetFilepath();
		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);		
	}
}

void RightPopupMenu::HandleAnimMenu(int id)
{
	if (id == MENU_ROTATE_LEFT_ID || id == MENU_ROTATE_RIGHT_ID)
	{
		std::string filepath = m_sprite->GetSymbol()->GetFilepath();
		assert(CharacterFileName::IsValidFilepath(filepath));
		CharacterFileName name(filepath);
		int dir = 1 + (name.GetField(CharacterFileName::FT_DIRECTION)[0] - '1');
		if (m_sprite->GetMirror().x) {
			dir = 10 - dir;
		}

		bool rot_left = id == MENU_ROTATE_LEFT_ID;
		if (rot_left) { --dir; }
		else { ++dir; }
		if (dir == 0) { dir = 8; }
		else if (dir == 9) { dir = 1; }

		ee::Symbol* symbol = m_stage->GetCharaDirs()->GetSymbolByDir(filepath, dir);
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

		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(item.GetFilepath());
		static_cast<ecomplex::Sprite*>(m_sprite)->SetSymbol(symbol);
	}
}

void RightPopupMenu::HandleLayerTagMenu(int id)
{
	ee::SpriteSelection* selection = m_stage->GetSpriteSelection();
	std::vector<ee::Sprite*> sprites;
	selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		ee::Sprite* spr = sprites[i];

		std::string tag = spr->GetTag();
		size_t p_begin = tag.find("layer=");
		if (p_begin != std::string::npos) {
			size_t p_end = tag.find(";", p_begin) + 1;
			tag.erase(tag.begin() + p_begin, tag.begin() + p_end);
		}

		if (id == MENU_COVER_LAYER_TAG_ID) {
			tag += std::string(COVER_LAYER_TAG) + ";";
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		} else if (id == MENU_TOP_LAYER_TAG_ID) {
			tag += std::string(TOP_LAYER_TAG) + ";";
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}

		spr->SetTag(tag);
	}
}

void RightPopupMenu::HandleMoveToLayerMenu(int id)
{
	int idx = id - MENU_MOVE_TO_LAYER_BEGIN_ID;	
	Layer* from = static_cast<LibraryPage*>(m_library->GetCurrPage())->GetLayer();
	ee::Layer* to = from->GetLayerMgr()->GetLayer(idx);
	
	ee::SpriteSelection* selection = m_stage->GetSpriteSelection();
	std::vector<ee::Sprite*> sprites;
	selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ee::Sprite* spr = sprites[i];
		from->RemoveSprite(spr);
		to->Insert(spr);
	}
}

void RightPopupMenu::FetchCandidateAnimFiles(const std::string& filepath)
{
	m_anim_files.clear();

	CharacterFileName name(filepath);

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files, ee::FileType::e_complex);

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