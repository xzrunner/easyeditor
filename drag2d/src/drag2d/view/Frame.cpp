#include "Frame.h"
#include "SettingsDialog.h"

#include "common/FileNameTools.h"
#include "common/Exception.h"
#include "common/config.h"
#include "common/Color.h"
#include "common/SettingData.h"
#include "common/StackTrace.h"
#include "view/ExceptionDlg.h"
#include "view/ConfirmDialog.h"
#include "view/FrameDropTarget.h"
#include "view/EditPanel.h"
#include "view/IStageCanvas.h"
#include "message/ClearSpriteSJ.h"
#include "message/ClearShapeSJ.h"
#include "message/ClearPanelSJ.h"
#include "render/DrawCallBatching.h"

#include <wx/filename.h>
#include <fstream>

namespace d2d
{

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::onNew)
	EVT_MENU(wxID_OPEN, Frame::onOpen)
	EVT_MENU(wxID_SAVE, Frame::onSave)
	EVT_MENU(wxID_SAVEAS, Frame::onSaveAs)

	EVT_MENU(ID_FULL_VIEWS, Frame::OnFullView)
	EVT_MENU(ID_SETTINGS, Frame::onSettings)

	EVT_MENU(wxID_EXIT, Frame::onQuit)
	
	EVT_CLOSE(Frame::onClose)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size /*wxDefaultSize, wxMAXIMIZE | wxCLOSE_BOX*/)
	, m_task(NULL)
	, m_filetag(filetag)
	, m_recent(this)
	, m_config(filetag.ToStdString())
{
	LoadWindowConfig();
	loadTmpInfo();
	initMenuBar();
#ifdef _DEBUG
	initStatueBar();

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
	saveTmpInfo();
}

void Frame::setTask(ITask* task)
{
	m_task = task;
}

void Frame::initWithFile(const wxString& path)
{
	Clear();
	m_curr_filename = path;
//	SetTitle(path);
	try {
		m_task->Load(path);
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::openFile(const wxString& filename)
{
	if (!m_task || filename.IsEmpty()) {
		return;
	}

	if (Config::Instance()->IsUseDTex()) {
		DrawCallBatching::Instance()->Clear();
	}

	Clear();

	m_curr_filename = filename;
	m_recent.insert(m_curr_filename);
	SetTitle(m_curr_filename);

	try {
		m_task->Load(m_curr_filename.c_str());
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::RefreshWithCurrFile()
{
//	m_task->clear();
	openFile(m_curr_filename);
}

void Frame::Clear()
{
	ClearSpriteSJ::Instance()->Clear();
	ClearShapeSJ::Instance()->Clear();
	ClearPanelSJ::Instance()->Clear();
}

void Frame::saveTmpInfo()
{
	std::string filename = wxFileName::GetCwd() + "\\.easy";
	Json::Value value;
	m_recent.save(value);
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void Frame::loadTmpInfo()
{
	std::string filename = wxFileName::GetCwd() + "\\.easy";
	if (wxFileName::FileExists(filename))
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filename.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		m_recent.load(value);
	}
}

void Frame::onNew(wxCommandEvent& event)
{
	if (!m_task) return;

	setCurrFilename();
	Clear();

	if (Config::Instance()->IsUseDTex()) {
		DrawCallBatching::Instance()->Clear();
	}
}

void Frame::onOpen(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
			GetFileFilter(), wxFD_OPEN);
		if (dlg.ShowModal() == wxID_OK)
		{
			openFile(dlg.GetPath());
		}
	} catch (Exception& e) {
		ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::onSave(wxCommandEvent& event)
{
	if (!m_task || m_curr_filename.empty()) return;

	try {
		wxMessageDialog* dlg = new wxMessageDialog(NULL, 
			wxT("Are you sure to save?"), wxT("Question"), 
			wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
		if (dlg->ShowModal() == wxID_YES)
		{
			SetTitle(m_curr_filename);
			m_task->Store(m_curr_filename.c_str());
		}
	} catch (Exception& e) {
		ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::onSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	try {
		wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, 
			GetFileFilter(), wxFD_SAVE);
		if (dlg.ShowModal() == wxID_OK)
		{
			wxString fixed = d2d::FilenameTools::getFilenameAddTag(dlg.GetPath(), m_filetag, "json");
			m_curr_filename = fixed;
			m_task->Store(fixed);
		}
	} catch (Exception& e) {
		ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::OnFullView(wxCommandEvent& event)
{
}

void Frame::onSettings(wxCommandEvent& event)
{
	SettingsDialog dlg(this);
	dlg.ShowModal();

	const Colorf& col = Config::Instance()->GetSettings().bg_color;
	const_cast<d2d::EditPanel*>(m_task->GetEditPanel())->GetCanvas()->SetBgColor(col);
}

wxString Frame::GetFileFilter() const
{
	return GetJsonFileFilter(m_filetag);
}

wxString Frame::GetJsonFileFilter(const wxString& file_tag)
{
	wxString fliter;
	fliter.Printf("JSON files (*_%s.json)|*_%s.json", file_tag, file_tag);
	return fliter;	
}

void Frame::onOpenRecent1(wxCommandEvent& event)
{
	wxString filename = m_recent.getFilename(0);
	openFile(filename);
}

void Frame::onOpenRecent2(wxCommandEvent& event)
{
	wxString filename = m_recent.getFilename(1);
	openFile(filename);
}

void Frame::onOpenRecent3(wxCommandEvent& event)
{
	wxString filename = m_recent.getFilename(2);
	openFile(filename);
}

void Frame::onOpenRecent4(wxCommandEvent& event)
{
	wxString filename = m_recent.getFilename(3);
	openFile(filename);
}

void Frame::onOpenRecent5(wxCommandEvent& event)
{
	wxString filename = m_recent.getFilename(4);
	openFile(filename);
}

void Frame::onQuit(wxCommandEvent& event)
{
	Close(true);
}

void Frame::onClose(wxCloseEvent& event)
{
	ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES) {
		if (!m_curr_filename.empty()) {
			m_task->Store(m_curr_filename.c_str());
		} else {
			onSaveAs(wxCommandEvent());
		}
		Destroy();
	} else if (val == wxID_NO) {
		Destroy();
	}
}

void Frame::initMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(initFileBar(), "&File");
	menuBar->Append(m_view_menu = initViewBar(), "&View");
	menuBar->Append(m_setting_menu = initSettingsBar(), "&Settings");
	menuBar->Append(m_code_menu = InitCodeBar(), "&Code");
	SetMenuBar(menuBar);
}

void Frame::initStatueBar()
{
	CreateStatusBar();
	SetStatusText( "Welcome to wxWidgets!" );
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
	fileMenu->AppendSubMenu(m_recent.getMenu(), wxT("Recent Files"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit"));
	return fileMenu;
}

wxMenu* Frame::initViewBar()
{
	wxMenu* viewMenu = new wxMenu;
	viewMenu->Append(ID_FULL_VIEWS, wxT("Full view\tF12"), wxT("Switch full view"));
//	viewMenu->Append(ID_EJ_PREVIEW, wxT("View"), wxT("View"));
	return viewMenu;
}

wxMenu* Frame::initSettingsBar()
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

		if (!d2d::FilenameTools::IsFileExist(str))
		{
			m_curr_filename = str;
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

//////////////////////////////////////////////////////////////////////////
// class Frame::RecentFiles
//////////////////////////////////////////////////////////////////////////

Frame::RecentFiles::
RecentFiles(wxFrame* frame)
	: m_frame(frame)
	, m_menu(new wxMenu)
{
}

void Frame::RecentFiles::
save(Json::Value& value) const
{
	for (int i = 0, n = m_files.size(); i < n; ++i)
	{
		value["recent"][i] = m_files[i].ToStdString();
	}
}

void Frame::RecentFiles::
load(const Json::Value& value)
{
	int i = 0;
	Json::Value v = value["recent"][i++];
	while (!v.isNull()) {
		insert(v.asString());
		v = value["recent"][i++];
	}
}

void Frame::RecentFiles::
insert(const wxString& filename)
{
	std::deque<wxString>::iterator itr = m_files.begin();
	for ( ; itr != m_files.end(); ++itr)
	{
		if (*itr == filename) {
			return;
		}
	}

	if (m_files.size() >= CAPACITY)
	{
		m_files.pop_front();

		while (m_menu->GetMenuItemCount() > 0) {
			m_menu->Remove(m_menu->FindItemByPosition(0));
		}

		std::deque<wxString>::iterator itr = m_files.begin();
		for ( ; itr != m_files.end(); ++itr)
		{
			insertOnlyMenu(*itr);
		}
	}

	m_files.push_back(filename);
	insertOnlyMenu(filename);
}

void Frame::RecentFiles::
insertOnlyMenu(const wxString& filename)
{
	int size = m_files.size();
	if (size > CAPACITY) {
		return;
	}
	int id = ID_RECENT_FILES + size;
	m_menu->Append(id, filename);
	if (size == 1) {
		m_frame->Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::onOpenRecent1));
	} else if (size == 2) {
		m_frame->Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::onOpenRecent2));
	} else if (size == 3) {
		m_frame->Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::onOpenRecent3));
	} else if (size == 4) {
		m_frame->Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::onOpenRecent4));
	} else if (size == 5) {
		m_frame->Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::onOpenRecent5));
	}
}

}