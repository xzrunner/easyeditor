#include "Frame.h"
#include "Task.h"
#include "Context.h"
#include "StagePanel.h"

#include <ee/FileHelper.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/ZoomViewOP.h>
#include <ee/StringHelper.h>

namespace edb
{

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::OnNew)
	EVT_MENU(wxID_OPEN, Frame::OnOpen)
	EVT_MENU(wxID_SAVE, Frame::OnSave)
	EVT_MENU(wxID_SAVEAS, Frame::OnSaveAs)

	EVT_MENU(ID_CONNECT, Frame::OnConnect)

	EVT_MENU(wxID_EXIT, Frame::OnQuit)
END_EVENT_TABLE()

static const std::string FILE_TAG = "edb";

Frame::Frame(const std::string& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
	m_task = Task::Create(this);
	InitMenuBar();
}

void Frame::OnNew(wxCommandEvent& event)
{
	SetCurrFilename();
	m_task->Clear();
}

void Frame::OnOpen(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILE_TAG + wxT(".json"), wxFD_OPEN);
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
		SetTitle(ee::FileHelper::GetFilename(m_curr_filename));
		m_task->StoreToFile(m_curr_filename.c_str());
	}
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILE_TAG + wxT(".json"), wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string fixed = ee::FileHelper::GetFilenameAddTag(dlg.GetPath().ToStdString(), FILE_TAG, "json");
		m_curr_filename = fixed;
		m_task->StoreToFile(fixed.c_str());
	}
}

void Frame::OnConnect(wxCommandEvent& event)
{
	ee::ZoomViewOP* op = dynamic_cast<ee::ZoomViewOP*>
		(Context::Instance()->stage->GetEditOP());
	if (op) {
		op->SetMouseMoveFocus(false);
	}

	wxDirDialog dlg(NULL, "Choose directory", wxEmptyString,
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		try {
//			Context::Instance()->stage->loadFromDir(dlg.GetPath().ToStdString());
			Context::Instance()->stage->LoadFromDirFast(dlg.GetPath().ToStdString());
		} catch (ee::Exception& e) {
			ee::ExceptionDlg dlg(this, e);
			dlg.ShowModal();
		}
	}

	if (op) {
		op->SetMouseMoveFocus(true);
	}
}

void Frame::OnQuit(wxCommandEvent& event)
{
	Close(true);
}

void Frame::InitMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(InitFileBar(), "&File");
	SetMenuBar(menuBar);
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
	fileMenu->Append(ID_CONNECT, wxT("&Connect"), wxT("Connect dir"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit GameFruits"));
	return fileMenu;
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
			std::string(".json");

		if (!ee::FileHelper::IsFileExist(str))
		{
			m_curr_filename = str;
			break;
		}
	}
}

}