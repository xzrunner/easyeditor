#include "Frame.h"

#include "Task.h"

#include <wx/splitter.h>

using namespace shootbubble;

BEGIN_EVENT_TABLE(Frame, wxFrame)
EVT_MENU(wxID_NEW, Frame::onNew)
EVT_MENU(wxID_OPEN, Frame::onOpen)
EVT_MENU(wxID_SAVE, Frame::onSave)
EVT_MENU(wxID_SAVEAS, Frame::onSaveAs)

EVT_MENU(wxID_EXIT, Frame::onQuit)
END_EVENT_TABLE()

static const wxString FILE_TAG = wxT("shootbubble");

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
		SetTitle(ee::FilenameTools::getFilename(dlg.GetPath()));
		m_task->loadFromFile(dlg.GetPath());
	}
}

void Frame::onSave(wxCommandEvent& event)
{
	if (!m_currFilename.empty())
	{
		SetTitle(ee::FilenameTools::getFilename(m_currFilename));
		m_task->storeToFile(m_currFilename);
	}
}

void Frame::onSaveAs(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILE_TAG + wxT(".json"), wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString fixed = ee::FilenameTools::getFilenameAddTag(dlg.GetPath(), FILE_TAG, "json");
		m_currFilename = fixed;
		m_task->storeToFile(fixed);
	}
}

void Frame::onQuit(wxCommandEvent& event)
{
	Close(true);
}

void Frame::initMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(initFileBar(), "&File");
	SetMenuBar(menuBar);
}

wxMenu* Frame::initFileBar()
{
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_NEW, wxT("&New\tCtrl+N"), wxT("Create a project"));
	fileMenu->Append(wxID_OPEN, wxT("&Open...\tCtrl+O"), wxT("Open a project"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Save the project"));
	fileMenu->Append(wxID_SAVEAS, wxT("&Save as...\tF11"), wxT("Save to a new file"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit GameFruits"));
	return fileMenu;
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

		if (!ee::FilenameTools::isExist(str))
		{
			m_currFilename = str;
			break;
		}
	}
}
