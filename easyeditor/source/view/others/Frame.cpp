#include "Frame.h"
#include "SettingsDialog.h"
#include "RecentFilesMenu.h"
#include "FileHelper.h"
#include "Exception.h"
#include "SettingData.h"
#include "StackTrace.h"
#include "ExceptionDlg.h"
#include "ConfirmDialog.h"
#include "FrameDropTarget.h"
#include "EditPanel.h"
#include "StageCanvas.h"
#include "Task.h"
#include "Config.h"
#include "sprite_msg.h"
#include "shape_msg.h"
#include "panel_msg.h"
#include "StringHelper.h"
#include "SymbolMgr.h"
#include "LogMgr.h"
#include "ReloadSymVisitor.h"
#include "Socket.h"
#include "EditedFileStack.h"
#include "PackIDMgr.h"

#include <gum/GUM_DTex.h>

#include <fstream>

namespace ee
{

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::OnNew)
	EVT_MENU(wxID_OPEN, Frame::OnOpen)
	EVT_MENU(wxID_SAVE, Frame::OnSave)
	EVT_MENU(wxID_SAVEAS, Frame::OnSaveAs)

	EVT_MENU(ID_FULL_VIEWS, Frame::OnFullView)
	EVT_MENU(ID_SETTINGS, Frame::OnSettings)
	EVT_MENU(ID_CONN_DEFAULT, Frame::OnConnDefault)
	EVT_MENU(ID_CONN_PC, Frame::OnConnPC)
	EVT_MENU(ID_CONN_IOS, Frame::OnConnIOS)
	EVT_MENU(ID_CONN_ANDROID, Frame::OnConnANDROID)

	EVT_MENU(wxID_EXIT, Frame::OnQuit)
	
	EVT_CLOSE(Frame::OnClose)
END_EVENT_TABLE()

Frame::Frame(const std::string& title, const std::string& filetag, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size /*wxDefaultSize, wxMAXIMIZE | wxCLOSE_BOX*/)
	, m_task(NULL)
	, m_filetag(filetag)
	, m_recent_menu(new RecentFilesMenu(ID_RECENT_FILENAME))
	, m_config(filetag)
{
	LoadWindowConfig();
	LoadTmpInfo();
	InitMenuBar();
	InitStatueBar();

#ifdef _DEBUG
	wxLog::SetActiveTarget(new wxLogWindow(this, _T("Log window")));
	m_log_chain = new wxLogChain(new wxLogStderr);
#else
	wxLog::SetLogLevel(0);
#endif

	StackTrace::InitUnhandledExceptionFilter();

	SetDropTarget(new FrameDropTarget(this));
}

Frame::~Frame()
{
#ifdef _DEBUG
	delete m_log_chain;
#endif

	StoreWindowConfig();
	SaveTmpInfo();

	delete m_recent_menu;
}

void Frame::SetTask(Task* task)
{
	m_task = task;
}

void Frame::InitWithFile(const std::string& path)
{
	Clear();
	EditedFileStack::Instance()->SetBottom(path);
//	SetTitle(path);
	try {
		m_task->Load(path.c_str());
	} catch (Exception& e) {
		ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::OpenFile(const std::string& filename)
{
	if (!m_task || filename.empty()) {
		return;
	}

	if (Config::Instance()->IsUseDTex()) {
		gum::DTex::Instance()->Clear();
	}

	Clear();

	EditedFileStack::Instance()->SetBottom(filename);
	m_recent_menu->Insert(filename);
	SetTitle(filename);

	try {
		m_task->Load(filename.c_str());
	} catch (Exception& e) {
		ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}

	const std::string& err_log = LogMgr::Instance()->GetException();
	if (!err_log.empty()) {
		ExceptionDlg dlg(this, err_log);
		dlg.ShowModal();
		LogMgr::Instance()->ClearException();
	}
}

void Frame::RefreshWithCurrFile()
{
//	m_task->clear();
	OpenFile(EditedFileStack::Instance()->GetBottom());

	SymbolMgr::Instance()->Traverse(ReloadSymVisitor());
}

void Frame::Clear()
{
	ClearSpriteSJ::Instance()->Clear();
	ClearShapeSJ::Instance()->Clear();
	ClearPanelSJ::Instance()->Clear();

	SymbolMgr::Instance()->Clear();
}

void Frame::SaveTmpInfo()
{
	std::string filename = FileHelper::GetAbsolutePath("\\.easy");
	Json::Value value;
	m_recent_menu->StoreToFile(value);
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void Frame::LoadTmpInfo()
{
	std::string filename = FileHelper::GetAbsolutePath("\\.easy");
	if (FileHelper::IsFileExist(filename))
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filename.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		m_recent_menu->LoadFromFile(value);
	}
}

void Frame::OnOpenRecentFile(wxCommandEvent& event)
{
	std::string filename = m_recent_menu->GetFilename(event.GetId());
	OpenFile(filename);
}

void Frame::OnNew(wxCommandEvent& event)
{
	if (!m_task) return;

	SetCurrFilename();
	Clear();

	if (Config::Instance()->IsUseDTex()) {
		gum::DTex::Instance()->Clear();
	}
}

void Frame::OnOpen(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
			GetFileFilter(), wxFD_OPEN);
		if (dlg.ShowModal() == wxID_OK) {
			OpenFile(dlg.GetPath().ToStdString());
		}
	} catch (Exception& e) {
		ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::OnSave(wxCommandEvent& event)
{
	std::string filepath = EditedFileStack::Instance()->GetBottom();
	if (!m_task || filepath.empty()) {
		return;
	}

	try {
		wxMessageDialog* dlg = new wxMessageDialog(NULL, 
			wxT("Are you sure to save?"), wxT("Question"), 
			wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
		if (dlg->ShowModal() == wxID_YES)
		{
			SetTitle(filepath);
			m_task->Store(filepath.c_str());
		}
	} catch (Exception& e) {
		ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::OnSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, 
			GetFileFilter(), wxFD_SAVE);
		if (dlg.ShowModal() == wxID_OK)
		{
			std::string fixed = FileHelper::GetFilenameAddTag(dlg.GetPath().ToStdString(), m_filetag, "json");
			EditedFileStack::Instance()->SetBottom(fixed);
			m_task->Store(fixed.c_str());
		}
	} catch (Exception& e) {
		ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::OnFullView(wxCommandEvent& event)
{
}

void Frame::OnSettings(wxCommandEvent& event)
{
	SettingsDialog dlg(this);
	dlg.ShowModal();

	const s2::Color& col = Config::Instance()->GetSettings().bg_color;
	const_cast<EditPanel*>(m_task->GetEditPanel())->GetCanvas()->SetBgColor(col);
}

void Frame::OnConnDefault(wxCommandEvent& event)
{
	// prepare id mgr
	std::string filepath = Config::Instance()->GetSettings().pack_id_dir + "\\pkg_cfg_only_ui.json";
	PackIDMgr::Instance()->Init(filepath, "pc");

	Socket::Instance()->ConnDefault();
}

void Frame::OnConnPC(wxCommandEvent& event)
{
	// prepare id mgr
	std::string filepath = Config::Instance()->GetSettings().pack_id_dir + "\\pkg_cfg_only_ui.json";
	PackIDMgr::Instance()->Init(filepath, "pc");

	wxString hostname = wxGetTextFromUser(
		_("Enter the address of the client:"),
		_("Connect ..."),
		_("localhost"));
	Socket::Instance()->ConnFromUser(hostname.ToStdString());
}

void Frame::OnConnIOS(wxCommandEvent& event)
{
	// prepare id mgr
	std::string filepath = Config::Instance()->GetSettings().pack_id_dir + "\\pkg_cfg_only_ui.json";
	PackIDMgr::Instance()->Init(filepath, "ios");

	wxString hostname = wxGetTextFromUser(
		_("Enter the address of the client:"),
		_("Connect ..."),
		_("localhost"));
	Socket::Instance()->ConnFromUser(hostname.ToStdString());
}

void Frame::OnConnANDROID(wxCommandEvent& event)
{
	// prepare id mgr
	std::string filepath = Config::Instance()->GetSettings().pack_id_dir + "\\pkg_cfg_only_ui.json";
	PackIDMgr::Instance()->Init(filepath, "android");

	wxString hostname = wxGetTextFromUser(
		_("Enter the address of the client:"),
		_("Connect ..."),
		_("localhost"));
	Socket::Instance()->ConnFromUser(hostname.ToStdString());
}

std::string Frame::GetFileFilter() const
{
	return ee::FileHelper::GetJsonFileFilter(m_filetag);
}

void Frame::OnQuit(wxCommandEvent& event)
{
	Close(true);
}

void Frame::OnClose(wxCloseEvent& event)
{
	ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES) 
	{
		std::string filepath = EditedFileStack::Instance()->GetBottom();
		if (!filepath.empty()) {
			m_task->Store(filepath.c_str());
		} else {
			OnSaveAs(wxCommandEvent());
		}
		Destroy();
	} 
	else if (val == wxID_NO) 
	{
		Destroy();
	}
}

void Frame::InitMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(InitFileBar(), "&File");
	menuBar->Append(m_view_menu = InitViewBar(), "&View");
	menuBar->Append(m_setting_menu = InitSettingsBar(), "&Settings");
	menuBar->Append(m_code_menu = InitCodeBar(), "&Code");
	menuBar->Append(m_conn_menu = InitConnBar(), "Connect");
	SetMenuBar(menuBar);
}

void Frame::InitStatueBar()
{
	CreateStatusBar();
	SetStatusText( "Welcome to wxWidgets!" );
}

wxMenu* Frame::InitFileBar()
{
	for (int i = 0; i < RecentFilesMenu::CAPACITY; ++i) {
		Bind(wxEVT_COMMAND_MENU_SELECTED, &Frame::OnOpenRecentFile, this, ID_RECENT_FILENAME + i);
	}

	wxMenu* menu = new wxMenu;
	menu->Append(wxID_NEW, wxT("&New\tCtrl+N"), wxT("Create a project"));
	menu->Append(wxID_OPEN, wxT("&Open...\tCtrl+O"), wxT("Open a project"));
	menu->AppendSeparator();
	menu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Save the project"));
	menu->Append(wxID_SAVEAS, wxT("&Save as...\tF11"), wxT("Save to a new file"));
	menu->AppendSeparator();
	menu->AppendSubMenu(m_recent_menu->GetMenu(), wxT("Recent Files"));
	menu->AppendSeparator();
	menu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit"));
	return menu;
}

wxMenu* Frame::InitViewBar()
{
	wxMenu* viewMenu = new wxMenu;
	viewMenu->Append(ID_FULL_VIEWS, wxT("Full view\tF12"), wxT("Switch full view"));
//	viewMenu->Append(ID_EJ_PREVIEW, wxT("View"), wxT("View"));
	return viewMenu;
}

wxMenu* Frame::InitSettingsBar()
{
	wxMenu* settingsMenu = new wxMenu;
	settingsMenu->Append(ID_SETTINGS, wxT("Base"), wxT("Base"));
	return settingsMenu;
}

wxMenu* Frame::InitCodeBar()
{
	wxMenu* menu = new wxMenu;
	return menu;
}

wxMenu* Frame::InitConnBar()
{
	wxMenu* menu = new wxMenu;
	menu->Append(ID_CONN_DEFAULT, "Default");
	menu->Append(ID_CONN_PC, "PC");
	menu->Append(ID_CONN_IOS, "IOS");
	menu->Append(ID_CONN_ANDROID, "Android");
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
			wxT("new") + 
			StringHelper::ToString(id++) + 
			wxT(".json");

		if (!FileHelper::IsFileExist(str))
		{
			EditedFileStack::Instance()->SetBottom(str);
			break;
		}
	}
}

void Frame::LoadWindowConfig()
{
	if (m_config.IsExist()) {
		const SpecialConfig::Window& wnd = m_config.GetWnd();
		SetSize(wxSize(wnd.width, wnd.height));
		SetPosition(wxPoint(wnd.left, wnd.top));
	}
}

void Frame::StoreWindowConfig()
{
	if (m_config.IsExist()) {
		SpecialConfig::Window wnd;

		wxSize size = GetSize();
		wnd.width = size.GetWidth();
		wnd.height = size.GetHeight();

		wxPoint pos = GetPosition();
		wnd.left = pos.x;
		wnd.top = pos.y;

		m_config.SetWnd(wnd);
	}
}

}