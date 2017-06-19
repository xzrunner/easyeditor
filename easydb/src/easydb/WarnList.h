#ifndef _EASYDB_WARN_LIST_H_
#define _EASYDB_WARN_LIST_H_

#include <wx/treectrl.h>

#include <vector>

namespace edb
{

class Database;
class MainList;

class WarnList : public wxTreeCtrl
{
public:
	WarnList(wxWindow* parent, MainList* main_list, const Database& db);

	void Build(const Database& db);

	void Clear() {}

private:
	void BuildNodeErr(const Database& db);

	void BuildDup(const Database& db);
	void InsertDupList(const Database& db, const std::vector<int>& list, wxTreeItemId parent);

	void OnItemActivated(wxTreeEvent& event);

private:
	enum
	{
		ID_CTRL = 1000,
	};

private:
	wxTreeItemId m_root;

	MainList* m_main_list;

	const Database& m_db;

	DECLARE_EVENT_TABLE()

}; // WarnList

}

#endif // _EASYDB_WARN_LIST_H_