#include "LibraryPanel.h"
#include "LibraryPage.h"

namespace lr
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
}

void LibraryPanel::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		LibraryPage* page = static_cast<LibraryPage*>(m_pages[i]);
		page->LoadFromFile(value, dir);
	}
}

void LibraryPanel::StoreToFile(Json::Value& value, const std::string& dir) const
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		LibraryPage* page = static_cast<LibraryPage*>(m_pages[i]);
		page->StoreToFile(value, dir);
	}
}

}