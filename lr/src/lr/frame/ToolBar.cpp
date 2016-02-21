#include "ToolBar.h"

#include <ee/LibraryPanel.h>

#include <wx/wx.h>

namespace lr
{

ToolBar::ToolBar(wxFrame* frame, int id)
	: m_library(NULL)
{
	const long STYLE = wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;
	m_toolbar = frame->CreateToolBar(STYLE, id);
	PopulateToolbar(id);
}

void ToolBar::OnClick(int idx)
{
	m_library->SetCurrPage(idx);
}

void ToolBar::PopulateToolbar(int id)
{
	m_toolbar->AddTool(++id, wxEmptyString, wxBITMAP(BMP_GROUND));
	m_toolbar->AddTool(++id, wxEmptyString, wxBITMAP(BMP_DECORATE));
	m_toolbar->AddTool(++id, wxEmptyString, wxBITMAP(BMP_UNIT));
	m_toolbar->AddSeparator();
	m_toolbar->AddTool(++id, wxEmptyString, wxBITMAP(BMP_POINT));
	m_toolbar->AddTool(++id, wxEmptyString, wxBITMAP(BMP_PATH));
	m_toolbar->AddTool(++id, wxEmptyString, wxBITMAP(BMP_AREA));
	m_toolbar->AddTool(++id, wxEmptyString, wxBITMAP(BMP_CAREA));
	m_toolbar->AddSeparator();
	m_toolbar->AddTool(++id, wxEmptyString, wxBITMAP(BMP_CAMERA));
	m_toolbar->AddTool(++id, wxEmptyString, wxBITMAP(BMP_LEVEL));

	m_toolbar->Realize();
}

}