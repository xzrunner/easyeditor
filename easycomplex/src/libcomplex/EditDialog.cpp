#include "EditDialog.h"
#include "StagePanel.h"
#include "FileSaver.h"
#include "LibraryPanel.h"
#include "Symbol.h"
#include "PropertySetting.h"

#include <wx/splitter.h>

namespace ecomplex
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::onCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Symbol* symbol)
	: wxDialog(parent, wxID_ANY, "Edit Complex", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(symbol)
{
	SetTitle(symbol->getFilepath());
	initLayout();
	loadSymbolInfo();
}

void EditDialog::initLayout()
{
// 		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
// 
// 		m_editPanel = new StagePanel(this, this, m_symbol);
// 		sizer->Add(m_editPanel, 5, wxEXPAND);
// 		
// 		SetSizer(sizer);

	//////////////////////////////////////////////////////////////////////////

	wxSplitterWindow* rightVerticalSplitter = new wxSplitterWindow(this);
	wxSplitterWindow* leftVerticalSplitter = new wxSplitterWindow(rightVerticalSplitter);
	wxSplitterWindow* leftHorizontalSplitter = new wxSplitterWindow(leftVerticalSplitter);

	ecomplex::LibraryPanel* library = new ecomplex::LibraryPanel(leftHorizontalSplitter);
//	library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());

	d2d::PropertySettingPanel* property 
		= new d2d::PropertySettingPanel(leftHorizontalSplitter);

	StagePanel* stage = new StagePanel(leftVerticalSplitter, this, m_symbol, property, 
		static_cast<ecomplex::LibraryPanel*>(library));
	m_stage = stage;
	property->setPropertySetting(new ecomplex::PropertySetting(stage, m_symbol));

//		ToolbarPanel* toolbar = new ToolbarPanel(rightVerticalSplitter, context->stage, context->property);

	d2d::ViewlistPanel* viewlist
		= new d2d::ViewlistPanel(rightVerticalSplitter, stage, stage, property);
	m_viewlist = viewlist;
	stage->setViewlist(viewlist);

	if (library || property)
	{
		if (library && property)
		{
			leftHorizontalSplitter->SetSashGravity(0.65f);
			leftHorizontalSplitter->SplitHorizontally(library, property);
		}
		leftVerticalSplitter->SetSashGravity(0.15f);
		leftVerticalSplitter->SplitVertically(leftHorizontalSplitter, stage);
	}

	if (viewlist)
	{
		rightVerticalSplitter->SetSashGravity(0.85f);
		rightVerticalSplitter->SplitVertically(leftVerticalSplitter, viewlist);
	}

//		m_root = rightVerticalSplitter;

	wxSize size = GetSize();
	size.SetWidth(size.GetWidth() + 1);
	SetSize(size);
}

void EditDialog::onCloseEvent(wxCloseEvent& event)
{
	onClose(!event.CanVeto(), wxID_CANCEL);

	//////////////////////////////////////////////////////////////////////////

// 	if (!m_stage->isDirty())
// 	{
// 		Destroy();
// 		return;
// 	}
// 
// 	d2d::ExitDlg dlg(this);
// 	int val = dlg.ShowModal();
// 	if (val == wxID_OK)
// 	{
// 		const wxString& filepath = m_symbol->getFilepath();
// 		FileSaver::store(filepath, m_symbol);
// 		m_symbol->RefreshThumbnail(filepath);
// 		d2d::SpriteFactory::Instance()->updateBoundings(*m_symbol);
// 	}
// 	else if (val == wxID_CANCEL)
// 	{
// 		m_symbol->loadFromFile(m_symbol->getFilepath());
// 	}
// 
// 	Destroy();
}

void EditDialog::onClose(bool force, int returncode)
{
	if (m_stage->isDirty())
	{
	 	d2d::ExitDlg dlg(this);
	 	int val = dlg.ShowModal();
	 	if (val == wxID_OK)
	 	{
	 		const wxString& filepath = m_symbol->getFilepath();
	 		FileSaver::store(filepath, m_symbol);
	 		m_symbol->RefreshThumbnail(filepath);
	 		d2d::SpriteFactory::Instance()->updateBoundings(*m_symbol);
	 	}
	 	else if (val == wxID_CANCEL)
	 	{
	 		m_symbol->loadFromFile(m_symbol->getFilepath());
	 	}
	}

	if(IsModal())
	{
		EndModal(returncode);
	}
	else
	{
		SetReturnCode(returncode);
		Hide();
	}

	if(force)
	{
		Destroy();
	}
}

void EditDialog::loadSymbolInfo()
{
	//m_library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
	for (size_t i = 0, n = m_symbol->m_sprites.size(); i < n; ++i)
		m_viewlist->insert(m_symbol->m_sprites[i]);

//		m_stage->loadHistoryList(filename, m_symbol->m_sprites);
}

}