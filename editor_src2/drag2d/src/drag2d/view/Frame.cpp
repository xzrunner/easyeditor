#include "Frame.h"
#include "SettingsDialog.h"

#include "common/FileNameTools.h"
#include "common/Exception.h"
#include "view/ExceptionDlg.h"
#include "view/ExitDlg.h"

#include <wx/filename.h>

namespace d2d
{

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::onNew)
	EVT_MENU(wxID_OPEN, Frame::onOpen)
	EVT_MENU(wxID_SAVE, Frame::onSave)
	EVT_MENU(wxID_SAVEAS, Frame::onSaveAs)
	
	EVT_MENU(ID_SETTINGS, Frame::onSettings)

	EVT_MENU(wxID_EXIT, Frame::onQuit)
	
	EVT_CLOSE(Frame::onClose)
END_EVENT_TABLE()

Frame::Frame(const wxString& title, const wxString& filetag)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600) /*wxDefaultSize, wxMAXIMIZE | wxCLOSE_BOX*/)
	, m_task(NULL)
	, m_filetag(filetag)
	, m_recent(this)
{
	loadTmpInfo();
	initMenuBar();
}

Frame::~Frame()
{
	saveTmpInfo();
}

void Frame::setTask(ITask* task)
{
	m_task = task;
}

void Frame::initWithFile(const wxString& path)
{
	m_task->clear();
	m_currFilename = path;
	SetTitle(path);
	try {
		m_task->load(path);
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
	}
}

void Frame::saveTmpInfo()
{
	wxString filename = wxFileName::GetCwd() + "\\.easy";
	Json::Value value;
	m_recent.save(value);
	Json::StyledStreamWriter writer;
	std::ofstream fout(filename.fn_str());
	writer.write(fout, value);
	fout.close();
}

void Frame::loadTmpInfo()
{
	wxString filename = wxFileName::GetCwd() + "\\.easy";
	if (wxFileName::FileExists(filename))
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filename.fn_str());
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
	m_task->clear();
}

void Frame::onOpen(wxCommandEvent& event)
{
	if (!m_task) return;

	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
		getFileFilter(), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		openFile(dlg.GetPath());
	}
}

void Frame::onSave(wxCommandEvent& event)
{
	if (!m_task || m_currFilename.empty()) return;

	wxMessageDialog* dlg = new wxMessageDialog(NULL, 
		wxT("Are you sure to save?"), wxT("Question"), 
		wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
	if (dlg->ShowModal() == wxID_YES)
	{
		SetTitle(m_currFilename);
		m_task->store(m_currFilename);
	}
}

void Frame::onSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, 
		getFileFilter(), wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString fixed = d2d::FilenameTools::getFilenameAddTag(dlg.GetPath(), m_filetag, "json");
		m_currFilename = fixed;
		m_task->store(fixed);
	}
}

void Frame::onSettings(wxCommandEvent& event)
{
	SettingsDialog dlg(this);
	dlg.ShowModal();
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
	if (!m_task->isDirty())
	{
		Destroy();
		return;
	}

	ExitDlg dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_OK)
		m_task->store(m_currFilename);

	Destroy();
}

void Frame::initMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(initFileBar(), "&File");
	menuBar->Append(m_view_menu = initViewBar(), "&View");
	menuBar->Append(m_setting_menu = initSettingsBar(), "&Settings");
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
	fileMenu->AppendSubMenu(m_recent.getMenu(), wxT("Recent Files"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit"));
	return fileMenu;
}

wxMenu* Frame::initViewBar()
{
	wxMenu* viewMenu = new wxMenu;
//	viewMenu->Append(ID_EJ_PREVIEW, wxT("View"), wxT("View"));
	return viewMenu;
}

wxMenu* Frame::initSettingsBar()
{
	wxMenu* settingsMenu = new wxMenu;
	settingsMenu->Append(ID_SETTINGS, wxT("Settings"), wxT("Settings"));
	return settingsMenu;
}

wxMenu* Frame::initHelpBar()
{
	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP, wxT("&About...\tF1"), wxT("Show about dialog"));
	return helpMenu;
}

wxString Frame::getFileFilter() const
{
	return wxT("*_") + m_filetag + wxT(".json");
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

void Frame::openFile(const wxString& filename)
{
	if (!m_task || filename.IsEmpty()) {
		return;
	}

	m_task->clear();

	m_currFilename = filename;
	m_recent.insert(m_currFilename);
	SetTitle(m_currFilename);

	try {
		m_task->load(m_currFilename);
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(this, e);
		dlg.ShowModal();
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