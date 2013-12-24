#include <wx/splitter.h>

#include "MainFrame.h"

#include "frame/Context.h"
#include "view/LibraryPanel.h"
#include "view/StagePanel.h"
#include "view/TimeLinePanel.h"
#include "view/PreviewDialog.h"

namespace eanim
{

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_NEW, MainFrame::onNew)
	EVT_MENU(wxID_OPEN, MainFrame::onOpen)
	EVT_MENU(wxID_SAVE, MainFrame::onSave)
	EVT_MENU(wxID_SAVEAS, MainFrame::onSaveAs)

	EVT_MENU(wxID_EXIT, MainFrame::onQuit)
// 	EVT_MENU(Menu_LanguageChinese, MainFrame::onSetLanguageCN)
// 	EVT_MENU(Menu_LanguageEnglish, MainFrame::onSetLanguageEN)
	EVT_MENU(Menu_Preview, MainFrame::onPreview)
	EVT_MENU(wxID_HELP, MainFrame::onAbout)
END_EVENT_TABLE()

LanguageEntry MainFrame::entries[] = 
{
	{ "语言", "Language" },
	{ "中文", "Chinese" },
	{ "英语", "English" }
};

const float MainFrame::SASH_GRAVITY_HOR = 0.8f;
const float MainFrame::SASH_GRAVITY_VERT = 0.1f;

MainFrame::MainFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
	setCurrFilename();
	initMenuBar();
	initWorkingFrame();
}

void MainFrame::onNew(wxCommandEvent& event)
{
	setCurrFilename();
	clear();
	refresh();
}

void MainFrame::onOpen(wxCommandEvent& event)
{
 	wxFileDialog dlg(this, wxT("添加现有数据"), wxEmptyString, wxEmptyString, wxT("*.ani"), wxFD_OPEN);
 	if (dlg.ShowModal() == wxID_OK)
 	{
		clear();

		m_currFilename = dlg.GetPath().ToStdString();

		std::ifstream fin(dlg.GetPath().fn_str());
		Context::Instance()->library->loadFromTextFile(fin);
		Context::Instance()->timeline->loadFromTextFile(fin);
		fin.close();

		refresh();
 	}
}

void MainFrame::onSave(wxCommandEvent& event)
{
	if (!m_currFilename.empty())
	{
		std::ofstream fout(m_currFilename.c_str());
		Context::Instance()->library->storeToTextFile(fout);
		Context::Instance()->timeline->storeToTextFile(fout);
		fout.close();
	}
}

void MainFrame::onSaveAs(wxCommandEvent& event)
{
 	wxFileDialog dlg(this, wxT("保存"), wxEmptyString, wxEmptyString, wxT("*.ani"), wxFD_SAVE);
 	if (dlg.ShowModal() == wxID_OK)
 	{
		std::ofstream fout(dlg.GetPath().fn_str());
		Context::Instance()->library->storeToTextFile(fout);
		Context::Instance()->timeline->storeToTextFile(fout);
		fout.close();
 	}
}

void MainFrame::onQuit(wxCommandEvent& event)
{
	Close(true);
}

void MainFrame::onAbout(wxCommandEvent& event)
{
	wxString msg;
	msg.Printf(wxT("Game animation tools.\n(c) zz, 2012"));
	wxMessageBox(msg, wxT("About FruitsAnimator"), wxOK | wxICON_INFORMATION, this);
}

// void MainFrame::onSetLanguageCN(wxCommandEvent& event)
// {
// 	currLanguage = Chinese;
// }
// 
// void MainFrame::onSetLanguageEN(wxCommandEvent& event)
// {
// 	currLanguage = English;
// }

void MainFrame::onPreview(wxCommandEvent& event)
{
	PreviewDialog dlg(this, Context::Instance()->timeline->getLayerMgr(), 
		Context::Instance()->timeline->getFPSVal());
	dlg.ShowModal();
	Context::Instance()->stage->resetCanvas();
}

void MainFrame::initMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(initFileBar(), "&File");
	menuBar->Append(initViewBar(), "&View");
	menuBar->Append(initHelpBar(), "&Help");	
	SetMenuBar(menuBar);
}

wxMenu* MainFrame::initFileBar()
{
	wxMenu* fileMenu = new wxMenu;
 	fileMenu->Append(wxID_NEW, wxT("&New\tCtrl+N"), wxT("Create a project"));
 	fileMenu->Append(wxID_OPEN, wxT("&Open...\tCtrl+O"), wxT("Open a project"));
 	fileMenu->AppendSeparator();
// 	fileMenu->Append(wxID_CLOSE, wxT("&Close\tCtrl+C"), wxT("Close the project"));
 	fileMenu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Save the project"));
 	fileMenu->Append(wxID_SAVEAS, wxT("&Save as...\tF11", wxT("Save to a new file")));
 	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit GameFruits"));
	return fileMenu;
}

wxMenu* MainFrame::initViewBar()
{
	wxMenu* viewMenu = new wxMenu;

// 	wxMenu* language = new wxMenu;
// 	language->Append(Menu_LanguageChinese, entries[Menu_LanguageChinese].text[currLanguage]);
// 	language->Append(Menu_LanguageEnglish, entries[Menu_LanguageEnglish].text[currLanguage]);
// 	viewMenu->Append(Menu_LanguageSetting, entries[Menu_LanguageSetting].text[currLanguage], language);

	viewMenu->Append(Menu_Preview, wxT("&Preview\tCtrl+Enter"), wxT("Play the animation"));

	return viewMenu;
}

wxMenu* MainFrame::initHelpBar()
{
	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP, wxT("&About...\tF1"), wxT("Show about dialog"));
	return helpMenu;
}

void MainFrame::initWorkingFrame()
{
	Context* context = Context::Instance();

	wxSplitterWindow* rootSplitter = new wxSplitterWindow(this);

	wxSplitterWindow* editSplitter = new wxSplitterWindow(rootSplitter);
	context->timeline = new TimeLinePanel(rootSplitter);
	rootSplitter->SetSashGravity(SASH_GRAVITY_HOR);
	rootSplitter->SplitHorizontally(editSplitter, context->timeline);

	context->library = new LibraryPanel(editSplitter);
	context->stage = new StagePanel(editSplitter);
	editSplitter->SetSashGravity(SASH_GRAVITY_VERT);
	editSplitter->SplitVertically(context->library, context->stage);
}

void MainFrame::clear()
{
	Context* context = Context::Instance();
	context->library->clear();
	context->timeline->clear();
	context->stage->clear();
}

void MainFrame::refresh()
{
	Context* context = Context::Instance();
	context->stage->loadCurrFrameSprites();
	context->library->Refresh();
	context->timeline->Refresh();
	context->stage->Refresh();
}

void MainFrame::setCurrFilename()
{
	int id = 0;
	while (true)
	{
		wxString str = wxT("new") + wxString::FromDouble(id++) + wxT(".ani");
		if (!d2d::FilenameTools::isExist(str))
		{
			m_currFilename = str.ToStdString();
			break;
		}
	}
}

} // eanim