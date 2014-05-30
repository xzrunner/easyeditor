#include "ResourceMgr.h"
#include "StagePanel.h"
#include "StageCanvas.h"
#include "LibraryPage.h"
#include "SymbolInfo.h"

#include <JSON/json.h>

namespace sg
{

ResourceMgr::ResourceMgr(StagePanel* stage, d2d::LibraryPanel* library)
	: m_stage(stage)
	, m_library(library)
{
}

void ResourceMgr::init()
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
		initAmountLimit(value);
		initBackground(value);
		initGrid(value);
 		initBuildings(value);
 		initArrow(value);
		initGrass(value);
	} catch (d2d::Exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void ResourceMgr::initLibraryFromBuildings()
{
	initLibraryFromBuildings(m_defenses, m_buildings.defenses);
	initLibraryFromBuildings(m_resources, m_buildings.resources);
	initLibraryFromBuildings(m_army, m_buildings.army);
}

int ResourceMgr::queryAmountLimit(const std::string& name) const
{
	int level = m_stage->getLevel();

	assert(level >= 0 && level < m_amount_limit.size());
	std::map<std::string, int>::const_iterator itr 
		= m_amount_limit[level].find(name);
	if (itr == m_amount_limit[level].end()) {
		return -1;
	} else {
		return itr->second;
	}
}

void ResourceMgr::initBackground(const Json::Value& value)
{
	std::string filepath = value["background"].asString();
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
	d2d::GLCanvas* canvas = m_stage->getCanvas();
	static_cast<StageCanvas*>(canvas)->setBackground(symbol);
	symbol->release();
	canvas->resetInitState();
}

void ResourceMgr::initGrid(const Json::Value& value)
{
	std::string filepath = value["grid"]["filepath"].asString();
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	float angle = value["grid"]["angle"].asInt();
	float scale = value["grid"]["scale"].asDouble();
	float alpha = value["grid"]["alpha"].asInt();
	sprite->setTransform(d2d::Vector(0, 0), angle * d2d::TRANS_DEG_TO_RAD);
	sprite->setScale(scale, scale);
	m_stage->m_grid = sprite;
	symbol->release();
	m_stage->getCanvas()->resetInitState();
}

void ResourceMgr::initArrow(const Json::Value& value)
{
	{
		std::string filepath = value["arrow"]["down_path"].asString();
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
		m_stage->m_arrow_down = symbol;
//		symbol->release();
	}
	{
		std::string filepath = value["arrow"]["right_path"].asString();
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
		m_stage->m_arrow_right = symbol;
//		symbol->release();
	}
	m_stage->getCanvas()->resetInitState();
}

void ResourceMgr::initGrass(const Json::Value& value)
{
	int i = 0;
	Json::Value grassVal = value["grass"]["levels"][i++];
	while (!grassVal.isNull()) {
		std::string filepath = grassVal["filepath"].asString();
		float scale = grassVal["scale"].asDouble();
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->setScale(scale, scale);
		m_stage->m_grass[i-1] = sprite;

		grassVal = value["grass"]["levels"][i++];
	}
}

void ResourceMgr::initAmountLimit(const Json::Value& value)
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

void ResourceMgr::initBuildings(const Json::Value& value)
{
	initBuildings(value["buildings"]["defenses"], m_buildings.defenses);
	initBuildings(value["buildings"]["resources"], m_buildings.resources);
	initBuildings(value["buildings"]["army"], m_buildings.army);

	m_defenses = new LibraryPage(m_library->getNotebook(), "Defenses");
	m_library->addPage(m_defenses);
	m_resources = new LibraryPage(m_library->getNotebook(), "Resources");
	m_library->addPage(m_resources);
	m_army = new LibraryPage(m_library->getNotebook(), "Army");
	m_library->addPage(m_army);	

	initLibraryFromBuildings();
}

void ResourceMgr::initBuildings(const Json::Value& value, 
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

void ResourceMgr::initLibraryFromBuildings(LibraryPage* library, 
										   const std::vector<Building*>& buildings)
{
	library->getList()->clear();

	int lv = m_stage->getLevel();
	for (int i = 0, n = buildings.size(); i < n; ++i)
	{
		const Building* b = buildings[i];
		
		std::string filepath;
		const Item* pItem = NULL;
		for (int j = 0, m = b->levels.size(); j < m; ++j) {
			const Item& item = b->levels[j];
			if (item.town_hall_level <= lv) {
				filepath = item.res_snapshoot_path;
				pItem = &item;
			}
		}

		if (!filepath.empty()) 
		{
			SymbolInfo* info = new SymbolInfo;
			info->size = b->size;
			info->remain = queryAmountLimit(pItem->building->name);
			info->iswall = (b->name == "Wall" ? true : false);
			assert(info->remain != -1);

			info->level = pItem->level;
			info->building = pItem->building;

			d2d::ISymbol* s = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
			s->setInfo(wxString::FromDouble(info->remain));
			s->setUserData(info);
			library->getList()->insert(s);
			s->release();
		}
	}
}

}