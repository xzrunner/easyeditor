#ifndef _LIBSHAPE_MAIN_DIALOG_INL_
#define _LIBSHAPE_MAIN_DIALOG_INL_

#include "MainDialog.h"
#include "ToolbarPanel.h"

#include <wx/splitter.h>

namespace libshape
{

template <typename TStage>
MainDialog<TStage>::MainDialog(wxWindow* parent, Symbol* symbol)
 	: wxDialog(parent, wxID_ANY, "Edit Shape", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_root(NULL)
	, m_stage(NULL)
{
	InitLayout(symbol);

	wxString filepath = d2d::FilenameTools::getFilenameAddTag(
		symbol->getFilepath(), libshape::FILE_TAG, "json");
	if (d2d::FilenameTools::isExist(filepath)) {
		m_stage->LoadFromFile(filepath.mb_str());
		m_toolbar->selectSuitableEditOP();
	}
}

template <typename TStage>
MainDialog<TStage>::~MainDialog()
{
//	m_stage->storeShapes();
	delete m_root;
}

template <typename TStage>
void MainDialog<TStage>::onSize(wxSizeEvent& event)
{
	Layout();
	Refresh();
}

template <typename TStage>
void MainDialog<TStage>::InitLayout(Symbol* symbol)
{
	wxSplitterWindow* vertical = new wxSplitterWindow(this);
	wxSplitterWindow* horizontal = new wxSplitterWindow(vertical);

	d2d::PropertySettingPanel* property = new d2d::PropertySettingPanel(horizontal);
	m_stage = new TStage(vertical, this, symbol);
	m_toolbar = new ToolbarPanel(horizontal, property, m_stage);

	horizontal->SetSashGravity(0.6f);
	horizontal->SplitHorizontally(m_toolbar, property);

	vertical->SetSashGravity(0.17f);
	vertical->SplitVertically(horizontal, m_stage);

	m_root = vertical;
}

}

#endif // _LIBSHAPE_MAIN_DIALOG_INL_