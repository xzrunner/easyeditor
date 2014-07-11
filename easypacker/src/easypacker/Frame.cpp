#include "Frame.h"
#include "Task.h"
#include "Love2dCode.h"
#include "config.h"

#include <wx/splitter.h>

using namespace epacker;

//static const wxString VERSION = wxT("0.13.0514");

enum MenuID
{
	ID_LOVE2D = 700
};

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::onNew)
	EVT_MENU(wxID_OPEN, Frame::onOpen)
	EVT_MENU(wxID_SAVE, Frame::onSave)
	EVT_MENU(wxID_SAVEAS, Frame::onSaveAs)

	EVT_MENU(wxID_EXIT, Frame::onQuit)
	//	EVT_MENU(wxID_HELP, Frame::onAbout)
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
 	wxString ext = wxT("*_") + wxString(FILE_TAG) + wxT(".json");
 	wxString filter = "Easypacker file ("+ext+")|"+ext+"|TP file (*.json)|*json";
	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_task->clear();
		m_currFilename = dlg.GetPath();
		SetTitle(d2d::FilenameTools::getFilename(dlg.GetPath()));
		try {
			m_task->loadFromFile(dlg.GetPath());
		} catch (d2d::Exception& e) {
			d2d::ExceptionDlg dlg(this, e);
			dlg.ShowModal();
		}
	}
}

void Frame::onSave(wxCommandEvent& event)
{
	if (!m_currFilename.empty())
	{
		SetTitle(d2d::FilenameTools::getFilename(m_currFilename));
		m_task->storeToFile(m_currFilename);
	}
}

void Frame::onSaveAs(wxCommandEvent& event)
{
	wxString ext = wxT("*_") + wxString(FILE_TAG) + wxT(".json");
	wxString filter = "Easypacker file ("+ext+")|"+ext+"|TP file (*.json)|*json";
	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, filter, wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
// 		wxString fixed = d2d::FilenameTools::getFilenameAddTag(dlg.GetPath(), FILE_TAG, "json");
// 		m_currFilename = fixed;
// 		m_task->storeToFile(fixed);

		m_task->storeToFile(dlg.GetPath());
	}
}

void Frame::onQuit(wxCommandEvent& event)
{
	Close(true);
}

// void Frame::onAbout(wxCommandEvent& event)
// {
// 	wxString msg;
// 	msg.Printf(wxT("Version: ") + VERSION);
// 	wxMessageBox(msg, wxT("About"), wxOK | wxICON_INFORMATION, this);
// }

void Frame::onCodeLove2d(wxCommandEvent& event)
{
	ebuilder::CodeDialog dlg(this);

	ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, wxT("packer.lua"));

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
	menuBar->Append(initCodesBar(), "&Codes");
	//	menuBar->Append(initHelpBar(), "&Help");	
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
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit EasyPacker"));
	return fileMenu;
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
