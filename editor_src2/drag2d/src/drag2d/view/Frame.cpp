#include "Frame.h"

#include "common/FileNameTools.h"
#include "common/Exception.h"
#include "view/ExceptionDlg.h"
#include "view/ExitDlg.h"

namespace d2d
{
	BEGIN_EVENT_TABLE(Frame, wxFrame)
		EVT_MENU(wxID_NEW, Frame::onNew)
		EVT_MENU(wxID_OPEN, Frame::onOpen)
		EVT_MENU(wxID_SAVE, Frame::onSave)
		EVT_MENU(wxID_SAVEAS, Frame::onSaveAs)

		EVT_MENU(wxID_EXIT, Frame::onQuit)
		
		EVT_CLOSE(Frame::onClose)
	END_EVENT_TABLE()

	Frame::Frame(const wxString& title, const wxString& filetag)
		: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600) /*wxDefaultSize, wxMAXIMIZE | wxCLOSE_BOX*/)
		, m_task(NULL)
		, m_filetag(filetag)
	{
		initMenuBar();
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
			m_task->clear();
			m_currFilename = dlg.GetPath();
			SetTitle(dlg.GetPath());
			try {
				m_task->load(dlg.GetPath());
			} catch (d2d::Exception& e) {
				d2d::ExceptionDlg dlg(this, e);
				dlg.ShowModal();
			}
		}
	}

	void Frame::onSave(wxCommandEvent& event)
	{
		if (!m_task) return;

		if (!m_currFilename.empty())
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
		fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit"));
		return fileMenu;
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
}