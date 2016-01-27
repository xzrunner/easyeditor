#ifndef _DRAG2D_RECENT_FILES_MENU_H_
#define _DRAG2D_RECENT_FILES_MENU_H_

#include <wx/menu.h>

#include <deque>
#include <json/json.h>

namespace d2d
{

class RecentFilesMenu
{
public:
	RecentFilesMenu(int id_start);

	void StoreToFile(Json::Value& value) const;
	void LoadFromFile(const Json::Value& value);

	void Insert(const std::string& filename);

	std::string GetFilename(int id) const;

	wxMenu* GetMenu() { return m_menu; }

public:
	static const int CAPACITY = 16;

private:
	wxMenu* m_menu;
	int m_id_start;

	std::deque<std::string> m_filenames;

}; // RecentFilesMenu

}

#endif // _DRAG2D_RECENT_FILES_MENU_H_