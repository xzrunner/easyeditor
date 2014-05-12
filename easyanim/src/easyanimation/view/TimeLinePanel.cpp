#include "TimeLinePanel.h"
#include "LayersPanel.h"
#include "KeysPanel.h"

#include "frame/Context.h"

#include <wx/splitter.h>

namespace eanim
{

const float TimeLinePanel::SASH_GRAVITY_VERT = 0.1f;

TimeLinePanel::TimeLinePanel(wxWindow* parent)
	: wxPanel(parent)
{
	initLayout();
}

void TimeLinePanel::clear()
{
	Context::Instance()->layers.clear();
	static_cast<KeysPanel*>(Context::Instance()->keysPanel)->setSelectPos(-1, -1);
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
	Context* context = Context::Instance();

	wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
	context->layersPanel = new LayersPanel(splitter);
	context->keysPanel = new KeysPanel(splitter);
	splitter->SetSashGravity(SASH_GRAVITY_VERT);
	splitter->SplitVertically(context->layersPanel, context->keysPanel);
	sizer->Add(splitter, 1, wxEXPAND);
}

} // eanim