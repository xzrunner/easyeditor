#include "BuildingCfg.h"
#include "StagePanel.h"
#include "StageCanvas.h"
#include "LibraryPage.h"
#include "SymbolExt.h"
#include "SymbolRender.h"

#include <JSON/json.h>

namespace sg
{

BuildingCfg::BuildingCfg(StagePanel* stage, d2d::LibraryPanel* library)
	: m_stage(stage)
	, m_library(library)
{
}

void BuildingCfg::InitAllData()
{
	const char* config_filepath = "config.json";

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(config_filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	try {
		InitRegion(value);
		InitAmountLimit(value);
		InitBackground(value);
		InitGrid(value);
 		InitBuildings(value);
 		InitArrow(value);
		InitGrass(value);
	} catch (d2d::Exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void BuildingCfg::ResetLibraryList()
{
	ResetLibraryList(m_defenses, m_buildings.defenses);
	ResetLibraryList(m_resources, m_buildings.resources);
	ResetLibraryList(m_army, m_buildings.army);
}

int BuildingCfg::QueryAmountLimit(const std::string& name, int level) const
{
	assert(level > 0 && level <= m_amount_limit.size());
	std::map<std::string, int>::const_iterator itr 
		= m_amount_limit[level-1].find(name);
	if (itr == m_amount_limit[level-1].end()) {
		return -1;
	} else {
		return itr->second;
	}
}

bool BuildingCfg::QueryAttackRegion(const std::string& name, int& max_region, int& min_region)
{
	std::map<std::string, std::pair<int, int> >::iterator itr 
		= m_map_region.find(name);
	if (itr == m_map_region.end()) {
		return false;
	}

	max_region = itr->second.first;
	min_region = itr->second.second;
	return true;
}

void BuildingCfg::InitBackground(const Json::Value& value)
{
	std::string filepath = value["background"].asString();
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
	d2d::GLCanvas* canvas = m_stage->getCanvas();
	static_cast<StageCanvas*>(canvas)->SetBackground(symbol);
	symbol->Release();
	canvas->resetViewport();
}

void BuildingCfg::InitGrid(const Json::Value& value)
{
	std::string filepath = value["grid"]["filepath"].asString();
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	float angle = value["grid"]["angle"].asInt();
	float scale = value["grid"]["scale"].asDouble();
	float alpha = value["grid"]["alpha"].asInt();
	sprite->setTransform(d2d::Vector(0, 0), angle * d2d::TRANS_DEG_TO_RAD);
	sprite->setScale(scale, scale);
	SymbolRender::Instance()->SetGrid(sprite);
	symbol->Release();
	m_stage->getCanvas()->resetViewport();
}

void BuildingCfg::InitArrow(const Json::Value& value)
{
	std::string filepath = value["arrow"]["down_path"].asString();
	d2d::ISymbol* down = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);

	filepath = value["arrow"]["right_path"].asString();
	d2d::ISymbol* right = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);

	SymbolRender::Instance()->SetArrow(down, right);

	m_stage->getCanvas()->resetViewport();
}

void BuildingCfg::InitGrass(const Json::Value& value)
{
	int i = 0;
	Json::Value grassVal = value["grass"]["levels"][i++];
	while (!grassVal.isNull()) {
		std::string filepath = grassVal["filepath"].asString();
		float scale = grassVal["scale"].asDouble();
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->setScale(scale, scale);
		SymbolRender::Instance()->SetGrass(i-1, sprite);

		grassVal = value["grass"]["levels"][i++];
	}
}

void BuildingCfg::InitAmountLimit(const Json::Value& value)
{
	int i = 0;
	Json::Value amountVal = value["amount"][i++];
	while (!amountVal.isNull()) {
		std::map<std::string, int> mapName2Amount;
		Json::ValueIterator itr = amountVal.begin();
		while (itr != amountVal.end()) {
			std::string key = itr.key().asString();
			int val = amountVal[key].asInt();
			mapName2Amount.insert(std::make_pair(key, val));
			++itr;
		}
		m_amount_limit.push_back(mapName2Amount);

		amountVal = value["amount"][i++];
	}
}

void BuildingCfg::InitRegion(const Json::Value& value)
{
	if (value["region"].isNull()) {
		return;
	}

	std::string filepath = value["region"]["filepath"].asString();
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
	
	SymbolRender::Instance()->SetRegion(symbol, value["region"]["size"].asInt());

	int idx = 0;
	Json::Value reg_val = value["attack_region"][idx++];
	while (!reg_val.isNull()) {
		std::string type = reg_val["type"].asString();
		int rmax = reg_val["max"].asInt(),
			rmin = reg_val["min"].asInt();
		m_map_region.insert(std::make_pair(type, std::make_pair(rmax, rmin)));
		reg_val = value["attack_region"][idx++];
	}
}

void BuildingCfg::InitBuildings(const Json::Value& value)
{
	InitBuildings(value["buildings"]["defenses"], m_buildings.defenses);
	InitBuildings(value["buildings"]["resources"], m_buildings.resources);
	InitBuildings(value["buildings"]["army"], m_buildings.army);

	LoadSymbolUserData();

	wxWindow* nb = m_library->GetNotebook();
	m_defenses = new LibraryPage(nb, "Defenses");
	m_library->AddPage(m_defenses);
	m_resources = new LibraryPage(nb, "Resources");
	m_library->AddPage(m_resources);
	m_army = new LibraryPage(nb, "Army");
	m_library->AddPage(m_army);	
	d2d::ILibraryPage* other = new d2d::LibraryImagePage(nb);
	m_library->AddPage(other);

	ResetLibraryList();
}

void BuildingCfg::InitBuildings(const Json::Value& value, 
								std::vector<Building*>& buildings)
{
	int i = 0;
	Json::Value buildingVal = value[i++];
	while (!buildingVal.isNull()) {
		Building* b = new Building;
		b->name = buildingVal["name"].asString();
		b->size = buildingVal["size"].asInt();
		int j = 0;
		Json::Value levelVal = buildingVal["levels"][j++];
		while (!levelVal.isNull()) {
			Item item;
			item.town_hall_level = levelVal["town_hall_level"].asInt();
			item.res_snapshoot_path = levelVal["res_snapshoot"].asString();
			item.level = j - 1;
			item.building = b;
			b->levels.push_back(item);
			levelVal = buildingVal["levels"][j++];
		}
		buildings.push_back(b);

		buildingVal = value[i++];
	}
}

void BuildingCfg::ResetLibraryList(LibraryPage* library, const std::vector<Building*>& buildings)
{
	library->GetList()->clear();

	int lv = m_stage->GetBaseLevel();

	for (int i = 0, n = buildings.size(); i < n; ++i)
	{
		const Building* b = buildings[i];
		
		std::string filepath;
		const Item* pItem = NULL;
		int level = 0;
		for (int j = 0, m = b->levels.size(); j < m; ++j) {
			const Item& item = b->levels[j];
			if (item.town_hall_level <= lv) {
				filepath = item.res_snapshoot_path;
				level = j + 1;
				pItem = &item;
			}
		}

		if (!filepath.empty()) 
		{
			d2d::ISymbol* s = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			assert(s->getUserData());
			SymbolExt* info = static_cast<SymbolExt*>(s->getUserData());
			info->remain = QueryAmountLimit(pItem->building->name, m_stage->GetBaseLevel());
			info->level = level;
			s->RefreshThumbnail(filepath);
 			s->SetInfo(wxString::FromDouble(info->remain));
			library->GetList()->insert(s);
			s->Release();
		}
	}
}

void BuildingCfg::LoadSymbolUserData()
{
	LoadSymbolUserData(m_buildings.defenses);
	LoadSymbolUserData(m_buildings.resources);
	LoadSymbolUserData(m_buildings.army);
}

void BuildingCfg::LoadSymbolUserData(const std::vector<Building*>& buildings)
{
	for (int i = 0, n = buildings.size(); i < n; ++i)
	{
		const Building* b = buildings[i];
		for (int j = 0, m = b->levels.size(); j < m; ++j) 
		{
			const Item& item = b->levels[j];

			SymbolExt* info = new SymbolExt;
			info->size = b->size;
			info->remain = QueryAmountLimit(item.building->name, item.town_hall_level);
			info->wall_type = (b->name == "Wall" ? 0 : -1);

			info->level = item.level;
			info->building = item.building;

			try {
				d2d::ISymbol* s = d2d::SymbolMgr::Instance()->FetchSymbol(item.res_snapshoot_path);
				s->RefreshThumbnail(item.res_snapshoot_path);
				s->SetInfo(wxString::FromDouble(info->remain));
				s->setUserData(info);
			} catch (d2d::Exception& e) {
				std::cerr << e.what() << std::endl;
			}
		}
	}
}

}