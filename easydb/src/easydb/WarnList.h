#ifndef _EASYDB_WARN_LIST_H_
#define _EASYDB_WARN_LIST_H_

#include <wx/treectrl.h>

#include <vector>

namespace edb
{

class Database;

class WarnList : public wxTreeCtrl
{
public:
	WarnList(wxWindow* parent);

	void Build(const Database& db);

	void Clear() {}

private:
	void BuildDup(const Database& db);
	void InsertDupList(const Database& db, const std::vector<int>& list, wxTreeItemId parent);

private:
	wxTreeItemId m_root;

//	DECLARE_EVENT_TABLE()

}; // WarnList

}

#endif // _EASYDB_WARN_LIST_H_