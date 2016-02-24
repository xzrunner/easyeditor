#include "Frame.h"
#include "Task.h"
#include "Love2dCode.h"
#include "config.h"

#include <ee/FileHelper.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

#include <wx/splitter.h>

namespace etexpacker
{

//static const std::string VERSION = wxT("0.13.0514");

enum MenuID
{
	ID_LOVE2D = 700
};

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::OnNew)
	EVT_MENU(wxID_OPEN, Frame::OnOpen)
	EVT_MENU(wxID_SAVE, Frame::OnSave)
	EVT_MENU(wxID_SAVEAS, Frame::OnSaveAs)

	EVT_MENU(wxID_EXIT, Frame::OnQuit)
	//	EVT_MENU(wxID_H_ELP, Frame::onAbout)
	EVT_MENU(ID_LOVE2D, Frame::OnCodeLove2d)
END_EVENT_TABLE()

Frame::Frame(const std::string& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
	m_task = Task::Create(this);
	InitMenuBar();
#ifdef _DEBUG
	InitStatueBar();
#endif
}

void Frame::OnNew(wxCommandEvent& event)
{
	SetCurrFilename();
	m_task->Clear();
}

void Frame::OnOpen(wxCommandEvent& event)
{
 	std::string ext = std::string("*_") + FILE_TAG + ".json";
 	std::string filter = "Easypacker file ("+ext+")|"+ext+"|TP file (*.json)|*json";
	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_task->Clear();
		m_curr_filename = dlg.GetPath();
		SetTitle(ee::FileHelper::GetFilename(dlg.GetPath().ToStdString()));
		try {
			m_task->LoadFromFile(dlg.GetPath());
		} catch (ee::Exception& e) {
			ee::ExceptionDlg dlg(this, e);
			dlg.ShowModal();
		}
	}
}

void Frame::OnSave(wxCommandEvent& event)
{
	if (!m_curr_filename.empty())
	{
		SetTitle(ee::FileHelper::GetFilename(m_curr_filename).c_str());
		m_task->StoreToFile(m_curr_filename.c_str());
	}
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
	std::string ext = std::string("*_") + FILE_TAG + ".json";
	std::string filter = "Easypacker file ("+ext+")|"+ext+"|TP file (*.json)|*json";
	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, filter, wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
// 		std::string fixed = ee::FileHelper::getFilenameAddTag(dlg.GetPath(), FILE_TAG, "json");
// 		m_currFilename = fixed;
// 		m_task->storeToFile(fixed);

		m_task->StoreToFile(dlg.GetPath());
	}
}

void Frame::OnQuit(wxCommandEvent& event)
{
	Close(true);
}

// void Frame::onAbout(wxCommandEvent& event)
// {
// 	wxString msg;
// 	msg.Printf(wxT("Version: ") + VERSION);
// 	wxMessageBox(msg, wxT("About"), wxOK | wxICON_INFORMATION, this);
// }

void Frame::OnCodeLove2d(wxCommandEvent& event)
{
	ebuilder::CodeDialog dlg(this);

	ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, "packer.lua");

	ebuilder::CodeGenerator gen;
	Love2dCode code(gen);
	code.Resolve();
	page->SetReadOnly(false);
	page->SetText(gen.toText());
	page->SetReadOnly(true);

	dlg.notebook->AddPage(page, page->getName());

	dlg.ShowModal();
}

void Frame::InitMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(InitFileBar(), "&File");
	menuBar->Append(InitCodesBar(), "&Codes");
	//	menuBar->Append(initHelpBar(), "&Help");	
	SetMenuBar(menuBar);
}

void Frame::InitStatueBar()
{
	CreateStatusBar();
	SetStatusText( "Welcome to wxWidgets!" );
}

wxMenu* Frame::InitFileBar()
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

wxMenu* Frame::InitCodesBar()
{
	wxMenu* menu = new wxMenu;
	menu->Append(ID_LOVE2D, wxT("love2d"), wxEmptyString);

	return menu;
}

wxMenu* Frame::InitHelpBar()
{
	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP, wxT("&About...\tF1"), wxT("Show about dialog"));
	return helpMenu;
}

void Frame::SetCurrFilename()
{
	int id = 0;
	while (true)
	{
		std::string str = 
			std::string("new") + 
			ee::StringHelper::ToString(id++) + 
			".json";

		if (!ee::FileHelper::IsFileExist(str))
		{
			m_curr_filename = str;
			break;
		}
	}
}

}
