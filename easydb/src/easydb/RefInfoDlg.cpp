#include "RefInfoDlg.h"
#include "DBHelper.h"
#include "LeafNode.h"
#include "Database.h"
#include "Utility.h"

#include <wx/utils.h>

#include <set>

namespace edb
{

RefInfoDlg::RefInfoDlg(wxWindow* parent, const Database* db, const LeafNode* leaf)
	: wxDialog(parent, wxID_ANY, wxT("Ref Info"))
	, m_db(db)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	InitLayout(sizer, leaf);

	SetSizer(sizer);

 	sizer->Layout();
 	Refresh(true);
}

void RefInfoDlg::InitLayout(wxSizer* sizer, const LeafNode* leaf)
{
	bool closure = DBHelper::IsTreeClosure(*m_db, leaf);

	wxString str;
	str.Printf("闭包: %s", closure ? "yes" : "no");
	sizer->Add(new wxStaticText(this, wxID_ANY, str));

	sizer->AddSpacer(10);

	wxSize sz = GetSize();
	ItemsPanel* panel = new ItemsPanel(this, sz.GetX(), sz.GetY(), m_db, leaf);
	sizer->Add(panel);
}

/************************************************************************/
/* class RefInfoDlg::ItemsPanel                                         */
/************************************************************************/

RefInfoDlg::ItemsPanel::ItemsPanel(wxWindow* parent, int width, int height,
								   const Database* db, const LeafNode* leaf)
	: wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(width, height))
	, m_db(db)
{
	SetScrollbars(1, 1, 200, 100, 0, 0);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	InitLayout(sizer, leaf);

	SetSizer(sizer);

 	sizer->Layout();
 	Refresh(true);
}

void RefInfoDlg::ItemsPanel::InitLayout(wxSizer* sizer, const LeafNode* leaf)
{
	const std::set<int>& in_nodes = leaf->GetNodes(true);

	wxString str;
	str.Printf("被引用数量 %d", in_nodes.size());
	sizer->Add(new wxStaticText(this, wxID_ANY, str));

	std::set<int>::const_iterator itr = in_nodes.begin();
	for ( ; itr != in_nodes.end(); ++itr) {
		wxSizer* sz = NodeToCtrl(*itr);
		if (sz) {
			sizer->Add(sz);
		}
	}

	sizer->AddSpacer(10);

	const std::set<int>& out_nodes = leaf->GetNodes(false);

	str.Printf("引用数量 %d", out_nodes.size());
	sizer->Add(new wxStaticText(this, wxID_ANY, str));

	itr = out_nodes.begin();
	for ( ; itr != out_nodes.end(); ++itr) {
		wxSizer* sz = NodeToCtrl(*itr);
		if (sz) {
			sizer->Add(sz);
		}
	}
}

wxSizer* RefInfoDlg::ItemsPanel::NodeToCtrl(int node_id)
{
	if (node_id == -1) {
		return NULL;
	}

	const Node* node = m_db->Fetch(node_id);
	if (node->Type() == NODE_INDEX) {
		return NULL;
	}

	wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);

	wxTextCtrl* text = new wxTextCtrl(this, wxID_ANY, node->GetPath(), wxDefaultPosition, wxSize(225, -1), wxTE_READONLY);
	sz->Add(text);

	wxButton* btn = new wxButton(this, wxID_ANY, "...", wxDefaultPosition, wxSize(25, 25));
	Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ItemsPanel::OnOpenFilepath));
	sz->Add(btn, 0, wxLEFT | wxRIGHT, 5);

	m_id2path.insert(std::make_pair(btn->GetId(), node->GetPath()));

	return sz;
}

void RefInfoDlg::ItemsPanel::OnOpenFilepath(wxCommandEvent& event)
{
	int id = event.GetId();
	std::map<int, std::string>::iterator itr = m_id2path.find(id);
	if (itr != m_id2path.end()) {
		Utility::OpenPath(m_db->GetDirPath() + "\\" + itr->second);
	}
}

}