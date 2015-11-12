#include "StagePanel.h"
#include "StageCanvas.h"
#include "ToolBarPanel.h"
#include "EditOP.h"
#include "TopPannels.h"
#include "TopLibraryPanel.h"
#include "TopToolbarPanel.h"
#include "widget_id.h"

#include <easycomplex.h>

namespace eui
{
namespace wrapper
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels)
	: UIStagePage(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), top_pannels->library->GetRawLibrary())
	, m_top_pannels(top_pannels)
{
	SetEditOP(static_cast<EditClipboxOP*>(new EditOP(this, top_pannels->property)));
	SetCanvas(new StageCanvas(this));

	d2d::LibraryPanel* library = top_pannels->library->GetRawLibrary();
	library->SetCanvas(GetCanvas());

	top_pannels->property->SetEditPanel(GetStageImpl());

	m_toolbar = new ToolbarPanel(top_pannels->toolbar);
	m_toolbar_idx = top_pannels->toolbar->AddToolbar(m_toolbar);

	UIStagePage::UnRegistSubjects(this);

	m_clipbox.xMin = m_clipbox.yMin = -200;
	m_clipbox.xMax = m_clipbox.yMax =  200;
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

	const Json::Value& cb_val = value["clipbox"];
	m_clipbox.xMin = cb_val["x"].asDouble();
	m_clipbox.yMax = cb_val["y"].asDouble();
	m_clipbox.xMax = m_clipbox.xMin + cb_val["w"].asDouble();
	m_clipbox.yMin = m_clipbox.yMax - cb_val["h"].asDouble();

	std::string items_filepath = value["items filepath"].asString();
	items_filepath = d2d::FilenameTools::getAbsolutePathFromFile(filename, items_filepath);
	ecomplex::Symbol items_complex;
	items_complex.LoadFromFile(items_filepath);
	for (int i = 0, n = items_complex.m_sprites.size(); i < n; ++i) {
		d2d::ISprite* spr = items_complex.m_sprites[i];
		d2d::InsertSpriteSJ::Instance()->Insert(spr);
	}
}

void StagePanel::StoreToFile(const char* filename) const
{
	std::string name = filename;
	name = name.substr(0, name.find_last_of('_'));

	std::string dir = d2d::FilenameTools::getFileDir(filename);

	// items complex
	ecomplex::Symbol items_complex;
	TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(items_complex.m_sprites));
	for_each(items_complex.m_sprites.begin(), items_complex.m_sprites.end(), d2d::RetainObjectFunctor<d2d::ISprite>());
	std::string items_path = name + "_items_complex[gen].json";
	items_complex.SetFilepath(items_path);
	ecomplex::FileStorer::Store(items_path.c_str(), &items_complex);

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
	value["items filepath"] = d2d::FilenameTools::getRelativePath(dir, items_path).ToStdString();
	value["wrapper filepath"] = d2d::FilenameTools::getRelativePath(dir, top_path).ToStdString();
	value["user type"] = m_toolbar->GetType();
	value["clipbox"]["w"] = m_clipbox.xLength();
	value["clipbox"]["h"] = m_clipbox.yLength();
	value["clipbox"]["x"] = m_clipbox.xMin;
	value["clipbox"]["y"] = m_clipbox.yMax;
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
		SetCanvasDirty();
		m_top_pannels->library->EnableUILibrary(false);
		UIStagePage::RegistSubjects(this);
	} else {
		GetSpriteSelection()->Clear();
		UIStagePage::UnRegistSubjects(this);
	}
}

}
}