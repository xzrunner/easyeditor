#include "TimeLinePanel.h"
#include "LayersPanel.h"
#include "KeysPanel.h"
#include "ViewMgr.h"

#include "dataset/DataMgr.h"

#include <wx/splitter.h>

namespace eanim
{

static const float SASH_GRAVITY_VERT = 0.1f;

TimeLinePanel::TimeLinePanel(wxWindow* parent)
	: wxPanel(parent)
{
	InitLayout();
}

void TimeLinePanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	InitTitle(sizer);
	InitSplitterWindow(sizer);
	SetSizer(sizer);
}

void TimeLinePanel::InitTitle(wxSizer* sizer)
{
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("Time Line"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);
}

void TimeLinePanel::InitSplitterWindow(wxSizer* sizer)
{
	wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
	LayersPanel* layers_panel = new LayersPanel(splitter);
	KeysPanel* keys_panel = new KeysPanel(splitter);
	splitter->SetSashGravity(SASH_GRAVITY_VERT);
	splitter->SplitVertically(layers_panel, keys_panel);
	sizer->Add(splitter, 1, wxEXPAND);

	ViewMgr::Instance()->layers = layers_panel;
	ViewMgr::Instance()->keys = keys_panel;
}

} // eanim