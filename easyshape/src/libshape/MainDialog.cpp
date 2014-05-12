
#ifndef LIBSHAPE_MAIN_DIALOG_CPP
#define LIBSHAPE_MAIN_DIALOG_CPP

#include "MainDialog.h"
#include "ToolbarPanel.h"

#include <wx/splitter.h>

using namespace libshape;

template <typename TStage>
MainDialog<TStage>::MainDialog(wxWindow* parent, d2d::ISymbol* symbol)
 	: wxDialog(parent, wxID_ANY, "Edit Shape", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(symbol)
	, m_sprite(NULL)
	, m_root(NULL)
	, m_stage(NULL)
{
	initLayout();
	m_stage->loadShapes();
	m_toolbar->selectSuitableEditOP();
}

template <typename TStage>
MainDialog<TStage>::MainDialog(wxWindow* parent, d2d::ISprite* sprite)
 	: wxDialog(parent, wxID_ANY, "Edit Shape", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(NULL)
	, m_sprite(sprite)
	, m_root(NULL)
	, m_stage(NULL)
{
	initLayout();
	m_stage->loadShapes();
	m_toolbar->selectSuitableEditOP();
}

template <typename TStage>
MainDialog<TStage>::~MainDialog()
{
	m_stage->storeShapes();
	delete m_root;
}

template <typename TStage>
void MainDialog<TStage>::onSize(wxSizeEvent& event)
{
	Layout();
	Refresh();
}

template <typename TStage>
void MainDialog<TStage>::initLayout()
{
	wxSplitterWindow* vertical = new wxSplitterWindow(this);
	wxSplitterWindow* horizontal = new wxSplitterWindow(vertical);

	d2d::PropertySettingPanel* property = new d2d::PropertySettingPanel(horizontal);
	m_stage = m_symbol ? new TStage(vertical, this, m_symbol) : new TStage(vertical, this, m_sprite);
	m_toolbar = new ToolbarPanel(horizontal, property, m_stage);

	horizontal->SetSashGravity(0.6f);
	horizontal->SplitHorizontally(m_toolbar, property);

	vertical->SetSashGravity(0.17f);
	vertical->SplitVertically(horizontal, m_stage);

	m_root = vertical;

 	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
 	sizer->Add(vertical, 1, wxEXPAND);
 	SetSizer(sizer);
}

#endif // LIBSHAPE_MAIN_DIALOG_CPP