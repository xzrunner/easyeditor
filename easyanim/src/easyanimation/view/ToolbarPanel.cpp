#include "ToolbarPanel.h"
#include "StagePanel.h"

#include "edit/CommonCMPT.h"
#include "edit/SkeletonCMPT.h"
#include "frame/Context.h"
#include "frame/FileIO.h"

namespace eanim
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage, 
						   d2d::PropertySettingPanel* property, bool vertical)
	: d2d::ToolbarPanel(parent, stage, vertical)
	, m_animChoice(NULL)
{
	addChild(new CommonCMPT(this, wxT("Common"), stage, property, vertical));
	addChild(new SkeletonCMPT(this, wxT("Skeleton"), stage, property, vertical));
	SetSizer(initLayout());	
}

void ToolbarPanel::addAnimChoice(const std::vector<std::string>& choices)
{
	wxSizer* sizer = GetSizer();

	if (m_animChoice) 
		delete m_animChoice;

	wxArrayString array;
	for (size_t i = 0, n = choices.size(); i < n; ++i)
		array.push_back(choices[i]);

	m_animChoice = new wxRadioBox(this, wxID_ANY, wxT("anims"), wxDefaultPosition, wxDefaultSize, array, 1, wxRA_SPECIFY_COLS);
	Connect(m_animChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(ToolbarPanel::onChangeAnim));
	sizer->Add(m_animChoice);

	Layout();
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(initChildrenLayout());
	return sizer;
}

void ToolbarPanel::onChangeAnim(wxCommandEvent& event)
{
	Context::Instance()->resource.choice = event.GetInt();
	FileIO::reload();
}
} // eanim