#ifndef _EASYDB_REF_INFO_DLG_H_
#define _EASYDB_REF_INFO_DLG_H_

#include <wx/wx.h>

#include <map>

namespace edb
{

class Database;
class LeafNode;

class RefInfoDlg : public wxDialog
{
public:
	RefInfoDlg(wxWindow* parent, const Database* db, const LeafNode* leaf);

private:
	void InitLayout(wxSizer* sizer, const LeafNode* leaf);

private:
	class ItemsPanel : public wxScrolledWindow
	{
	public:
		ItemsPanel(wxWindow* parent, int width, int height,
			const Database* db, const LeafNode* leaf);

	private:
		void InitLayout(wxSizer* sizer, const LeafNode* leaf);

		wxSizer* NodeToCtrl(int node_id);

		void OnOpenFilepath(wxCommandEvent& event);

	private:
		const Database* m_db;

		std::map<int, std::string> m_id2path;

	}; // ItemsPanel

private:
	const Database* m_db;

}; // RefInfoDlg

}

#endif // _EASYDB_REF_INFO_DLG_H_