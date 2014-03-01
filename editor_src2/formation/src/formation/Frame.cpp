#include "Frame.h"

#include "Context.h"
#include "Task.h"
#include "PreviewDialog.h"
#include "StagePanel.h"
#include "Love2dCode.h"

#include <wx/splitter.h>

using namespace formation;

static const wxString FILE_TAG = wxT("formation");

enum MenuID
{
	ID_LOVE2D = 700
};

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::onNew)
	EVT_MENU(wxID_OPEN, Frame::onOpen)
	EVT_MENU(wxID_SAVE, Frame::onSave)
	EVT_MENU(wxID_SAVEAS, Frame::onSaveAs)

	EVT_MENU(Menu_Preview, Frame::onPreview)
	EVT_MENU(wxID_EXIT, Frame::onQuit)
	EVT_MENU(ID_LOVE2D, Frame::onCodeLove2d)
END_EVENT_TABLE()

Frame::Frame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
	m_task = Task::create(this);
	initMenuBar();
}

void Frame::onNew(wxCommandEvent& event)
{
	setCurrFilename();
	m_task->clear();
}

void Frame::onOpen(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILE_TAG + wxT(".json"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_task->clear();
		m_currFilename = dlg.GetPath();
		SetTitle(d2d::FilenameTools::getFilename(dlg.GetPath()));
		m_task->loadFromTextFile(dlg.GetPath());
	}
}

void Frame::onSave(wxCommandEvent& event)
{
	if (!m_currFilename.empty())
	{
		SetTitle(d2d::FilenameTools::getFilename(m_currFilename));
		m_task->storeToTextFile(m_currFilename);
	}
}

void Frame::onSaveAs(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILE_TAG + wxT(".json"), wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString fixed = d2d::FilenameTools::getFilenameAddTag(dlg.GetPath(), FILE_TAG, "json");
		m_currFilename = fixed;
		m_task->storeToTextFile(fixed);
	}
}

void Frame::onPreview(wxCommandEvent& event)
{
	PreviewDialog dlg(this);
	dlg.ShowModal();
	Context::Instance()->stage->resetCanvas();
}

void Frame::onQuit(wxCommandEvent& event)
{
	Close(true);
}

void Frame::onCodeLove2d(wxCommandEvent& event)
{
	ebuilder::CodeDialog dlg(this);

	ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, wxT("level.lua"));

	ebuilder::CodeGenerator gen;
	Love2dCode code(gen);
	code.resolve();
	page->SetReadOnly(false);
	page->SetText(gen.toText());
	page->SetReadOnly(true);

	dlg.notebook->AddPage(page, page->getName());

	dlg.ShowModal();
}

void Frame::initMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(initFileBar(), "&File");
	menuBar->Append(initViewBar(), "&View");
	menuBar->Append(initCodesBar(), "&Codes");
	SetMenuBar(menuBar);
}

wxMenu* Frame::initFileBar()
{
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_NEW, wxT("&New\tCtrl+N"), wxT("Create a project"));
	fileMenu->Append(wxID_OPEN, wxT("&Open...\tCtrl+O"), wxT("Open a project"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Save the project"));
	fileMenu->Append(wxID_SAVEAS, wxT("&Save as...\tF11", wxT("Save to a new file")));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit GameFruits"));
	return fileMenu;
}

wxMenu* Frame::initViewBar()
{
	wxMenu* viewMenu = new wxMenu;

	viewMenu->Append(Menu_Preview, wxT("&Preview\tCtrl+Enter"), wxT("Preview"));

	return viewMenu;
}

wxMenu* Frame::initCodesBar()
{
	wxMenu* menu = new wxMenu;
	menu->Append(ID_LOVE2D, wxT("love2d"), wxEmptyString);

	return menu;
}

wxMenu* Frame::initHelpBar()
{
	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP, wxT("&About...\tF1"), wxT("Show about dialog"));
	return helpMenu;
}

void Frame::setCurrFilename()
{
	int id = 0;
	while (true)
	{
		wxString str = 
			wxT("new") + 
			wxString::FromDouble(id++) + 
			wxT(".json");

		if (!d2d::FilenameTools::isExist(str))
		{
			m_currFilename = str;
			break;
		}
	}
}
