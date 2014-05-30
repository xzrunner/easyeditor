#ifndef _SG_RESOURCE_MGR_H_
#define _SG_RESOURCE_MGR_H_

#include <JSON/json.h>
#include <drag2d.h>

namespace sg
{

class LibraryPage;
class StagePanel;

class ResourceMgr
{
public:
	ResourceMgr(StagePanel* stage, d2d::LibraryPanel* library);

	void init();

	void initLibraryFromBuildings();

private:
	struct Item
	{
		int town_hall_level;
		std::string res_snapshoot_path;
	}; // Item

	struct Building
	{
		std::string name;
		int size;
		std::vector<Item> levels;
	}; // Building

	struct AllBuilding
	{
		std::vector<Building> defenses;
		std::vector<Building> resources;
		std::vector<Building> army;
	}; // AllBuilding

private:
	void initBackground(const Json::Value& value);
	void initGrid(const Json::Value& value);
	void initArrow(const Json::Value& value);
	void initGrass(const Json::Value& value);

	void initBuildings(const Json::Value& value);
	void initBuildings(const Json::Value& value, 
		std::vector<Building>& buildings);
	void initLibraryFromBuildings(LibraryPage* library, 
		const std::vector<Building>& buildings);

private:
	StagePanel* m_stage;

	d2d::LibraryPanel* m_library;

	AllBuilding m_buildings;

	LibraryPage *m_defenses, *m_resources, *m_army;

}; // ResourceMgr

}

#endif // _SG_RESOURCE_MGR_H_
