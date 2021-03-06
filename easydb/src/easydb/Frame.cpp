#include "Frame.h"
#include "Task.h"
#include "StagePanel.h"
#include "Database.h"
#include "MainList.h"
#include "MainPanel.h"
#include "SearchPanel.h"
#include "SearchList.h"
#include "WarnPanel.h"
#include "WarnList.h"
#include "Snapshoot.h"

#include <ee/FileHelper.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/ZoomViewOP.h>
#include <ee/StringHelper.h>

#include <gum/StringHelper.h>

namespace edb
{

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::OnNew)
	EVT_MENU(wxID_OPEN, Frame::OnOpen)
	EVT_MENU(wxID_SAVE, Frame::OnSave)
	EVT_MENU(wxID_SAVEAS, Frame::OnSaveAs)

	EVT_MENU(ID_CONNECT, Frame::OnConnect)
	EVT_MENU(ID_SEARCH, Frame::OnSearch)
	EVT_MENU(ID_SAME_IMAGE, Frame::OnSameImage)
	EVT_MENU(ID_BUILD_SNAPSHOOT, Frame::OnBuildSnapshoot)

	EVT_MENU(wxID_EXIT, Frame::OnQuit)
END_EVENT_TABLE()

static const std::string FILE_TAG = "edb";

Frame::Frame(const std::string& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
	m_task = Task::Create(this);
	InitMenuBar();

#ifdef _DEBUG
	wxLog::SetActiveTarget(new wxLogWindow(this, _T("Log window")));
	m_log_chain = new wxLogChain(new wxLogStderr);
#else
	wxLog::SetLogLevel(0);
#endif
}

Frame::~Frame()
{
#ifdef _DEBUG
	delete m_log_chain;
#endif
}

void Frame::OnNew(wxCommandEvent& event)
{
	SetCurrFilename();
	m_task->Clear();
}

void Frame::OnOpen(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, "EASY DB files (*.edb)|*.edb", wxFD_OPEN);
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
	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, wxT(".edb"), wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string fixed = dlg.GetPath().ToStdString();
		m_curr_filename = fixed;
		m_task->StoreToFile(fixed.c_str());
	}
}

void Frame::OnConnect(wxCommandEvent& event)
{
	auto op = dynamic_cast<ee::ZoomViewOP>
		(m_task->GetStagePanel()->GetEditOP());
	if (op) {
		op->SetMouseMoveFocus(false);
	}

	wxDirDialog dlg(NULL, "Choose directory", wxEmptyString,
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		try {
			std::string dirpath = dlg.GetPath().ToStdString();
			Database* db = m_task->GetStagePanel()->GetDB();
			db->Build(dirpath);
			m_task->GetMainPanel()->GetList()->Build(*db);
			m_task->GetWarnPanel()->GetList()->Build(*db);
		} catch (ee::Exception& e) {
			ee::ExceptionDlg dlg(this, e);
			dlg.ShowModal();
		}
	}

	if (op) {
		op->SetMouseMoveFocus(true);
	}
}

void Frame::OnSearch(wxCommandEvent& event)
{
	wxTextEntryDialog dlg(this, "Search");
	if (dlg.ShowModal()) 
	{
		std::string str = dlg.GetValue().ToStdString();
		gum::StringHelper::ToLower(str);
		m_task->GetSearchPanel()->GetList()->OnSearch(str);
	}
}

void Frame::OnSameImage(wxCommandEvent& event)
{
	wxTextEntryDialog dlg(this, "Same Val 1-100");
	if (dlg.ShowModal()) 
	{
		std::string str = dlg.GetValue().ToStdString();
		int val;
		gum::StringHelper::FromString(str, val);
		if (val >= 1 && val <= 100) {
			m_task->GetSearchPanel()->GetList()->OnSearchSameImages(val * 0.01f);			
		}
	}
}

void Frame::OnBuildSnapshoot(wxCommandEvent& event)
{
	wxDirDialog dlg(NULL, "output directory", wxEmptyString,
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK) {
		Snapshoot::Build(*m_task->GetStagePanel()->GetDB(), dlg.GetPath().ToStdString());
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
	menuBar->Append(InitToolBar(), "&Tool");
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

wxMenu* Frame::InitToolBar()
{
	wxMenu* tool = new wxMenu;
	tool->Append(ID_SEARCH, wxT("&Search...\tCtrl+F"), wxT("Search"));
	tool->Append(ID_SAME_IMAGE, wxT("Same Image"), wxT("Same Image"));
	tool->Append(ID_BUILD_SNAPSHOOT, wxT("����"), wxT("����"));
	return tool;
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
			std::string(".edb");

		if (!ee::FileHelper::IsFileExist(str))
		{
			m_curr_filename = str;
			break;
		}
	}
}

}