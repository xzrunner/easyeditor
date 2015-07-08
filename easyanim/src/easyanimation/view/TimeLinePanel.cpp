#include "TimeLinePanel.h"
#include "LayersPanel.h"
#include "KeysPanel.h"

#include "frame/Controller.h"

#include <wx/splitter.h>

namespace eanim
{

const float TimeLinePanel::SASH_GRAVITY_VERT = 0.1f;

TimeLinePanel::TimeLinePanel(wxWindow* parent, Controller* ctrl)
	: wxPanel(parent)
	, m_ctrl(ctrl)
{
	initLayout();
}

void TimeLinePanel::clear()
{
	m_ctrl->ClearAllLayer();
	m_ctrl->GetKeysPanel()->SetSelectPos(-1, -1);
}

void TimeLinePanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	initTitle(sizer);
	initSplitterWindow(sizer);
	SetSizer(sizer);
}

void TimeLinePanel::initTitle(wxSizer* sizer)
{
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("Time Line"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);
}

void TimeLinePanel::initSplitterWindow(wxSizer* sizer)
{
	wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
	LayersPanel* layers_panel = new LayersPanel(splitter, m_ctrl);
	KeysPanel* keys_panel = new KeysPanel(splitter, m_ctrl);
	splitter->SetSashGravity(SASH_GRAVITY_VERT);
	splitter->SplitVertically(layers_panel, keys_panel);
	sizer->Add(splitter, 1, wxEXPAND);

	m_ctrl->SetTimeLinePanel(layers_panel, keys_panel);
}

} // eanim