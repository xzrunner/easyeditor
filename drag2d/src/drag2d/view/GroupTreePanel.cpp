#include "GroupTreePanel.h"
#include "GroupTreeCtrl.h"
#include "GroupTreeImpl.h"
#include "GroupTreeIO.h"

#include "dataset/Group.h"
#include "common/visitors.h"

#include <wx/wx.h>

#include <fstream>

namespace d2d
{

GroupTreePanel::GroupTreePanel(wxWindow* parent, MultiSpritesImpl* sprites_impl,
							   ViewPanelMgr* view_panel_mgr)
	: wxPanel(parent, wxID_ANY)
	, m_sprite_impl(sprites_impl)
{
	m_grouptree = new GroupTreeCtrl(this, sprites_impl, view_panel_mgr);

	InitLayout();
}

void GroupTreePanel::SelectSprite(ISprite* spr)
{
	GroupTreeImpl::QuerySpriteVisitor visitor(m_grouptree, spr);
	m_grouptree->Traverse(visitor);
	wxTreeItemId id = visitor.GetItemID();
	if (id.IsOk()) {
		m_grouptree->SelectItem(id);
	}
}

void GroupTreePanel::SelectMultiSprites(SpriteSelection* selection)
{
	std::vector<ISprite*> sprites;
	selection->Traverse(FetchAllVisitor<ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		SelectSprite(sprites[i]);
	}
}

void GroupTreePanel::ReorderSprite(ISprite* spr, bool up)
{
	GroupTreeImpl::QuerySpriteVisitor visitor(m_grouptree, spr);
	m_grouptree->Traverse(visitor);
	wxTreeItemId id = visitor.GetItemID();
	if (id.IsOk()) {
		m_grouptree->ReorderItem(id, up);
	}
}

void GroupTreePanel::InsertSprite(ISprite* spr)
{
	m_grouptree->AddSprite(spr);
}

void GroupTreePanel::RemoveSprite(ISprite* spr)
{
	std::ofstream fout("del_debug.txt", std::ios::app);
	fout << "GroupTreePanel::RemoveSprite 0" << std::endl;

	m_grouptree->Remove(spr);
	fout << "GroupTreePanel::RemoveSprite 1" << std::endl;

	fout.close();
}

void GroupTreePanel::StoreToFile(Json::Value& value) const
{
	m_grouptree->Traverse(GroupTreeImpl::StoreVisitor(m_grouptree, value));
}

void GroupTreePanel::LoadFromFile(const Json::Value& value)
{
	GroupTreeIO io(m_grouptree, m_sprite_impl);
	io.Load(value);
}

void GroupTreePanel::Remove(ISprite* sprite)
{
	m_grouptree->Remove(sprite);
}

void GroupTreePanel::InitGroups(const std::map<std::string, std::vector<d2d::ISprite*> >& groups)
{
	if (groups.empty()) {
		return;
	}

	std::vector<wxTreeItemId> groups_id;
	std::map<std::string, std::vector<d2d::ISprite*> >::const_iterator itr
		= groups.begin();
	for ( ; itr != groups.end(); ++itr)
	{
		wxTreeItemId id = m_grouptree->AddNode(itr->first, m_grouptree->GetRootID());
		groups_id.push_back(id);
		for (int i = 0, n = itr->second.size(); i < n; ++i) {
			m_grouptree->AddSprite(id, itr->second[i]);
		}
	}

	for (int i = 0, n = groups_id.size(); i < n; ++i) {
		m_grouptree->Traverse(groups_id[i], GroupTreeImpl::SetVisibleVisitor(m_grouptree, false));
		m_grouptree->Traverse(groups_id[i], GroupTreeImpl::SetEditableVisitor(m_grouptree, false));
	}
	m_grouptree->Traverse(groups_id[0], GroupTreeImpl::SetVisibleVisitor(m_grouptree, true));
	m_grouptree->Traverse(groups_id[0], GroupTreeImpl::SetEditableVisitor(m_grouptree, true));
}

void GroupTreePanel::Clear()
{
	m_grouptree->Clear();
}

void GroupTreePanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(InitHeaderLayout(), 0, wxEXPAND);
	sizer->Add(m_grouptree, 1, wxEXPAND);

	SetSizer(sizer);
}

wxSizer* GroupTreePanel::InitHeaderLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	// title
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT(" Group"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 1, wxALIGN_LEFT);

	// add
	wxButton* btn_add = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(20, 20));
	Connect(btn_add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GroupTreePanel::OnAddPress));
	sizer->Add(btn_add, 0, wxLEFT | wxRIGHT, 5);

	// del
	wxButton* btn_del = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(20, 20));
	Connect(btn_del->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GroupTreePanel::OnDelPress));
	sizer->Add(btn_del, 0, wxLEFT | wxRIGHT, 5);

	sizer->Fit(this);

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