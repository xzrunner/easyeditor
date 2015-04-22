#ifndef _LR_LIBRARY_PANEL_H_
#define _LR_LIBRARY_PANEL_H_

#include <drag2d.h>

namespace lr
{

class LibraryPanel : public d2d::LibraryPanel
{
public:
	LibraryPanel(wxWindow* parent);

	void LoadFromFile(const Json::Value& value, const std::string& dir);
	void StoreToFile(Json::Value& value, const std::string& dir) const;

}; // LibraryPanel

}

#endif // _LR_LIBRARY_PANEL_H_