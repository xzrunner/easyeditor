#include "GroupTreePanel.h"
#include "GroupTreeCtrl.h"

#include <wx/wx.h>

namespace d2d
{

GroupTreePanel::GroupTreePanel(wxWindow* parent, MultiSpritesImpl* sprites_impl)
	: wxPanel(parent, wxID_ANY)
{
	m_grouptree = new GroupTreeCtrl(this, sprites_impl);

	InitLayout();
}

void GroupTreePanel::StoreToFile(Json::Value& value) const
{
	m_grouptree->StoreToFile(value);
}

void GroupTreePanel::LoadFromFile(const Json::Value& value)
{
	m_grouptree->LoadFromFile(value);
}

void GroupTreePanel::Remove(ISprite* sprite)
{
	m_grouptree->Remove(sprite);
}

void GroupTreePanel::InitGroups(const std::map<std::string, std::vector<d2d::ISprite*> >& groups)
{
	std::vector<wxTreeItemId> groups_id;
	std::map<std::string, std::vector<d2d::ISprite*> >::const_iterator itr
		= groups.begin();
	for ( ; itr != groups.end(); ++itr)
	{
		wxTreeItemId id = m_grouptree->AddNode(itr->first);
		groups_id.push_back(id);
		for (int i = 0, n = itr->second.size(); i < n; ++i) {
			m_grouptree->AddSprite(id, itr->second[i]);
		}
	}

	for (int i = 0, n = groups_id.size(); i < n; ++i) {
		m_grouptree->Visible(groups_id[i], false);
		m_grouptree->Editable(groups_id[i], false);
	}
	if (!groups_id.empty()) {
		m_grouptree->Visible(groups_id[0], true);
		m_grouptree->Editable(groups_id[0], true);
	}
}

void GroupTreePanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(InitButtonsLayout(), 0, wxALIGN_RIGHT);

	sizer->Add(m_grouptree, 1, wxEXPAND);

	SetSizer(sizer);
}

wxSizer* GroupTreePanel::InitButtonsLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* btn_add = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(20, 20));
	Connect(btn_add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GroupTreePanel::OnAddPress));
	sizer->Add(btn_add, 0, wxLEFT | wxRIGHT, 5);

	wxButton* btn_del = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(20, 20));
	Connect(btn_del->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GroupTreePanel::OnDelPress));
	sizer->Add(btn_del, 0, wxLEFT | wxRIGHT, 5);

	return sizer;
}

void GroupTreePanel::OnAddPress(wxCommandEvent& event)
{
	m_grouptree->AddNode();
}

void GroupTreePanel::OnDelPress(wxCommandEvent& event)
{
	m_grouptree->DelNode();
}

}