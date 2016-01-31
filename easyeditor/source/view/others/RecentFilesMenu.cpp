#include "RecentFilesMenu.h"

namespace ee
{

RecentFilesMenu::RecentFilesMenu(int id_start)
	: m_id_start(id_start)
{
	m_menu = new wxMenu();
}

void RecentFilesMenu::StoreToFile(Json::Value& value) const
{
	for (int i = 0, n = m_filenames.size(); i < n; ++i) {
		value["recent"][i] = m_filenames[i];
	}
}

void RecentFilesMenu::LoadFromFile(const Json::Value& value)
{
	int i = 0;
	Json::Value v = value["recent"][i++];
	while (!v.isNull()) {
		Insert(v.asString());
		v = value["recent"][i++];
	}
}

void RecentFilesMenu::Insert(const std::string& filename)
{
	// query
	std::deque<std::string>::iterator itr = m_filenames.begin();
	for ( ; itr != m_filenames.end(); ++itr) {
		if (*itr == filename) {
			return;
		}
	}

	// replace
	if (m_filenames.size() >= CAPACITY) {
		m_filenames.pop_front();
		while (m_menu->GetMenuItemCount() > 0) {
			m_menu->Remove(m_menu->FindItemByPosition(0));
		}
		for (int i = 0, n = m_filenames.size(); i < n; ++i) {
			int id = i + m_id_start;
			m_menu->Append(id, m_filenames[i]);
		}
	}
	m_filenames.push_back(filename);
	m_menu->Append(m_id_start + m_filenames.size() - 1, filename);
}

std::string RecentFilesMenu::GetFilename(int id) const
{
	int idx = id - m_id_start;
	if (idx < 0 || idx >= m_filenames.size()) {
		return "";
	} else {
		return m_filenames[idx];
	}
}

}