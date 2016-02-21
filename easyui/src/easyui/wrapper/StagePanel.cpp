#include "StagePanel.h"
#include "StageCanvas.h"
#include "ToolBarPanel.h"
#include "EditOP.h"
#include "TopPannels.h"
#include "TopLibraryPanel.h"
#include "TopToolbarPanel.h"
#include "widget_id.h"

#include <ee/wrap_StagePanel.h>
#include <ee/FileHelper.h>
#include <ee/sprite_msg.h>
#include <ee/panel_msg.h>
#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>

#include <easycomplex.h>

#include <fstream>
#include <algorithm>

namespace ee { extern StageModule MODULE_STAGE; }

namespace eui
{
namespace wrapper
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, TopPannels* top_pannels)
	: UIStagePage(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), top_pannels->library->GetRawLibrary())
	, m_top_pannels(top_pannels)
{
	SetEditOP(static_cast<EditClipboxOP*>(new EditOP(this, top_pannels->property)));
	SetCanvas(new StageCanvas(this, glctx));

	ee::LibraryPanel* library = top_pannels->library->GetRawLibrary();

	top_pannels->property->SetEditPanel(GetStageImpl());

	m_toolbar = new ToolbarPanel(top_pannels->toolbar);
	m_toolbar_idx = top_pannels->toolbar->AddToolbar(m_toolbar);

	m_clipbox.xmin = m_clipbox.ymin = -200;
	m_clipbox.xmax = m_clipbox.ymax =  200;

	ee::MODULE_STAGE.impl = this;
}

void StagePanel::LoadFromFile(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string type = value["type"].asString();
	if (!type.empty() && !is_widget(type, ID_WRAPPER)) {
		return;
	}

	std::string user_type = value["user type"].asString();
	m_toolbar->SetType(user_type);

	std::string tag = value["tag"].asString();
	m_toolbar->SetTag(tag);

	const Json::Value& cb_val = value["clipbox"];
	m_clipbox.xmin = cb_val["x"].asDouble();
	m_clipbox.ymax = cb_val["y"].asDouble();
	m_clipbox.xmax = m_clipbox.xmin + cb_val["w"].asDouble();
	m_clipbox.ymin = m_clipbox.ymax - cb_val["h"].asDouble();

	std::string items_filepath = value["items filepath"].asString();
	items_filepath = ee::FileHelper::GetAbsolutePathFromFile(filename, items_filepath);
	ecomplex::Symbol items_complex;
	items_complex.LoadFromFile(items_filepath);
	for (int i = 0, n = items_complex.m_sprites.size(); i < n; ++i) {
		ee::Sprite* spr = items_complex.m_sprites[i];
		ee::InsertSpriteSJ::Instance()->Insert(spr);
	}
}

void StagePanel::StoreToFile(const char* filename) const
{
	std::string name = filename;
	name = name.substr(0, name.find_last_of('_'));

	std::string dir = ee::FileHelper::GetFileDir(filename);

	// items complex
	ecomplex::Symbol items_complex;
	TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(items_complex.m_sprites));
	for_each(items_complex.m_sprites.begin(), items_complex.m_sprites.end(), ee::RetainObjectFunctor<ee::Sprite>());
	std::string items_path = name + "_items_complex[gen].json";
	items_complex.SetFilepath(items_path);
	ecomplex::FileStorer::Store(items_path.c_str(), &items_complex);
	items_complex.InitBounding();

	// wrapper complex
	ecomplex::Sprite items_sprite(&items_complex);
	items_sprite.name = "anchor";
	ecomplex::Symbol wrapper_complex;
	wrapper_complex.m_clipbox = m_clipbox;
	wrapper_complex.m_sprites.push_back(&items_sprite);
	items_sprite.Retain();
	std::string top_path = name + "_wrapper_complex[gen].json";
	wrapper_complex.SetFilepath(top_path);
	ecomplex::FileStorer::Store(top_path.c_str(), &wrapper_complex);

	// ui
	std::string ui_path = filename;
	Json::Value value;
	value["type"] = get_widget_desc(ID_WRAPPER);
	value["items filepath"] = ee::FileHelper::GetRelativePath(dir, items_path);
	value["wrapper filepath"] = ee::FileHelper::GetRelativePath(dir, top_path);
	value["user type"] = m_toolbar->GetType();
	value["tag"] = m_toolbar->GetTag();
	value["clipbox"]["w"] = m_clipbox.Width();
	value["clipbox"]["h"] = m_clipbox.Height();
	value["clipbox"]["x"] = m_clipbox.xmin;
	value["clipbox"]["y"] = m_clipbox.ymax;
	ee::Rect r = items_sprite.GetRect();
	value["children"]["w"] = r.Width();
	value["children"]["h"] = r.Height();
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(ui_path.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void StagePanel::EnablePage(bool enable)
{
	if (enable) {
		m_top_pannels->toolbar->EnableToolbar(m_toolbar_idx);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		m_top_pannels->library->EnableUILibrary(false);
	} else {
		GetSpriteSelection()->Clear();
	}

	EnableObserve(enable);
	GetStageImpl()->EnableObserve(enable);
}

}
}