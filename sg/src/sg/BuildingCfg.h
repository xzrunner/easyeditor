#ifndef _SG_BUILDING_CFG_H_
#define _SG_BUILDING_CFG_H_

#include <JSON/json.h>


namespace sg
{

class LibraryPage;
class StagePanel;

class BuildingCfg
{
public:
	BuildingCfg(StagePanel* stage, ee::LibraryPanel* library);

	void InitAllData();

	void ResetLibraryList();

	int QueryAmountLimit(const std::string& name, int level) const;

	bool QueryAttackRegion(const std::string& name, int& max_region, int& min_region);

public:
	struct Building;
	struct Item
	{
		int town_hall_level;
		std::string res_snapshoot_path;

		int level;
		Building* building;
	}; // Item

	struct Building
	{
		std::string name;
		int size;
		std::vector<Item> levels;
	}; // Building

	struct AllBuilding
	{
		std::vector<Building*> defenses;
		std::vector<Building*> resources;
		std::vector<Building*> army;
	}; // AllBuilding

private:
	void InitBackground(const Json::Value& value);
	void InitGrid(const Json::Value& value);
	void InitArrow(const Json::Value& value);
	void InitGrass(const Json::Value& value);
	void InitAmountLimit(const Json::Value& value);
	void InitRegion(const Json::Value& value);

	void InitBuildings(const Json::Value& value);
	void InitBuildings(const Json::Value& value, 
		std::vector<Building*>& buildings);

	void ResetLibraryList(LibraryPage* library, 
		const std::vector<Building*>& buildings);

	void LoadSymbolUserData();
	void LoadSymbolUserData(const std::vector<Building*>& buildings);

private:
	StagePanel* m_stage;

	ee::LibraryPanel* m_library;

	AllBuilding m_buildings;

	LibraryPage *m_defenses, *m_resources, *m_army;

	std::vector<std::map<std::string, int> > m_amount_limit;

	std::map<std::string, std::pair<int, int> > m_map_region;

}; // BuildingCfg

}

#endif // _SG_BUILDING_CFG_H_
