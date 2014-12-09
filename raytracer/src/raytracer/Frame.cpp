#include "Frame.h"
#include "Canvas.h"
#include "main.xpm"

namespace rt
{

//IDs for menu items
enum
{
	Menu_File_Quit = 100,
	Menu_File_Open,
	Menu_File_Save,
	Menu_Render_Start,
	Menu_Render_Pause,
	Menu_Render_Resume
};

BEGIN_EVENT_TABLE( Frame, wxFrame )
   EVT_MENU( Menu_Render_Start, Frame::OnRenderStart )
   EVT_MENU( Menu_Render_Pause, Frame::OnRenderPause )
   EVT_MENU( Menu_Render_Resume, Frame::OnRenderResume )
   EVT_MENU( Menu_File_Save, Frame::OnSaveFile )
   EVT_MENU( Menu_File_Open, Frame::OnOpenFile )
   EVT_MENU( Menu_File_Quit, Frame::OnQuit )
   EVT_COMMAND(ID_RENDER_COMPLETED, wxEVT_RENDER, Frame::OnRenderCompleted)
END_EVENT_TABLE()

Frame::Frame(const wxPoint& pos, const wxSize& size)
	: wxFrame((wxFrame *)NULL, -1, wxT( "Ray Tracer" ), pos, size)
{
	wxMenu *menuFile = new wxMenu;

	menuFile->Append(Menu_File_Open, wxT("&Open..."   ));
	menuFile->Append(Menu_File_Save, wxT("&Save As..."));
	menuFile->AppendSeparator();
	menuFile->Append(Menu_File_Quit, wxT("E&xit"));

	menuFile->Enable(menuFile->FindItem(wxT("&Save As...")), FALSE);

	wxMenu *menuRender = new wxMenu;

	menuRender->Append(Menu_Render_Start , wxT("&Start" ));
	menuRender->Append(Menu_Render_Pause , wxT("&Pause" ));
	menuRender->Append(Menu_Render_Resume, wxT("&Resume"));

	menuRender->Enable(menuRender->FindItem(wxT("&Start" )), TRUE );
	menuRender->Enable(menuRender->FindItem(wxT("&Pause" )), FALSE);
	menuRender->Enable(menuRender->FindItem(wxT("&Resume")), FALSE);

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile  , wxT("&File"  ));
	menuBar->Append(menuRender, wxT("&Render"));

	SetMenuBar( menuBar );

	m_canvas = new Canvas(this);

	CreateStatusBar();
	SetStatusText(wxT("Ready"));

	wxIcon icon(main_xpm);
	SetIcon(icon);

	wxStatusBar* statusBar = GetStatusBar();
	int widths[] = {150,300};
	statusBar->SetFieldsCount(2, widths);
}

void Frame::OnQuit( wxCommandEvent& WXUNUSED( event ) )
{
	Close();
}

void Frame::OnSaveFile( wxCommandEvent& WXUNUSED( event ) )
{
	wxString caption = wxT("Save File");

	wxString wildcard = wxT("*.png;*.jpg;*.bmp;*.tif");

	wxString defaultDir = wxEmptyString;
	wxString defaultFilename = wxT("render.bmp");

	wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard,
		wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	dialog.SetPath(m_current_path);

	if (dialog.ShowModal() == wxID_OK)
	{
		m_current_path = dialog.GetPath();

		wxImage theImage = m_canvas->GetImage();
		theImage.SaveFile(m_current_path);
	}
}

void Frame::OnOpenFile( wxCommandEvent& WXUNUSED( event ) )
{
	wxString caption = wxT("Choose a file");

	wxString wildcard = wxT("*.png;*.jpg;*.bmp;*.tif");

	wxString defaultDir = wxEmptyString;
	wxString defaultFilename = wxEmptyString;

	wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard,
		wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	dialog.SetPath(m_current_path);

	if (dialog.ShowModal() == wxID_OK)
	{
		m_current_path = dialog.GetPath();
		wxImage* theImage = new wxImage();
		theImage->LoadFile(m_current_path);

		if (!theImage->IsOk())
		{
			delete theImage;
			wxMessageBox(wxT("Sorry, could not load file."));
		}
		else
		{
			m_canvas->SetImage(*theImage);
			wxMenu* menuFile = GetMenuBar()->GetMenu(0);
			menuFile->Enable(menuFile->FindItem(wxT( "&Save As..."  )) , TRUE);
		}
	}
}

void Frame::OnRenderStart( wxCommandEvent& WXUNUSED( event ) )
{
	wxMenu* menu = GetMenuBar()->GetMenu(1);
	menu->Enable(menu->FindItem(wxT("&Start" )), FALSE);
	menu->Enable(menu->FindItem(wxT("&Pause" )), TRUE );
	menu->Enable(menu->FindItem(wxT("&Resume")), FALSE);

	m_canvas->renderStart();

	wxMenu* menuFile = GetMenuBar()->GetMenu(0);
	menuFile->Enable(menuFile->FindItem(wxT( "&Open..."   )), FALSE);
	menuFile->Enable(menuFile->FindItem(wxT( "&Save As...")), TRUE );
}

void Frame::OnRenderCompleted( wxCommandEvent& event )
{
	wxMenu* menu = GetMenuBar()->GetMenu(1);
	menu->Enable(menu->FindItem(wxT("&Start" )), TRUE );
	menu->Enable(menu->FindItem(wxT("&Pause" )), FALSE);
	menu->Enable(menu->FindItem(wxT("&Resume")), FALSE);

	wxMenu* menuFile = GetMenuBar()->GetMenu(0);
	menuFile->Enable(menuFile->FindItem(wxT("&Open...")), TRUE);

	SetStatusText(wxT("Rendering complete"));
}

void Frame::OnRenderPause( wxCommandEvent& event )
{
	wxMenu* menu = GetMenuBar()->GetMenu(1);
	menu->Enable(menu->FindItem(wxT("&Start" )), FALSE);
	menu->Enable(menu->FindItem(wxT("&Pause" )), FALSE);
	menu->Enable(menu->FindItem(wxT("&Resume")), TRUE );

	m_canvas->renderPause();

	SetStatusText( wxT( "Rendering paused" ) );
}

void Frame::OnRenderResume( wxCommandEvent& event )
{
	wxMenu* menu = GetMenuBar()->GetMenu(1);
	menu->Enable(menu->FindItem(wxT("&Start" )), FALSE);
	menu->Enable(menu->FindItem(wxT("&Pause" )), TRUE );
	menu->Enable(menu->FindItem(wxT("&Resume")), FALSE);

	m_canvas->renderResume();

	SetStatusText(wxT("Rendering..."));
}

}