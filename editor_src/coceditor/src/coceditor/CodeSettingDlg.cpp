#include "CodeSettingDlg.h"

#include "Context.h"

#include <wx/dir.h>

namespace coceditor
{

CodeSettingDlg::CodeSettingDlg(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, wxT("Setting"))
{
	initLayout();
}

void CodeSettingDlg::initLayout()
{
	Context* context = Context::Instance();

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	{
		wxSizer* scaleSizer = new wxBoxSizer(wxHORIZONTAL);

		wxStaticText* fpsText = new wxStaticText(this, wxID_ANY, wxT("scale:"));
		scaleSizer->Add(fpsText, 0, wxLEFT, 30);
		wxSpinCtrl* fpsCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 200, 100);
		Connect(fpsCtrl->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(CodeSettingDlg::onSetScale));
		scaleSizer->Add(fpsCtrl, 0, wxLEFT, 5);

		sizer->Add(scaleSizer);
	}
	sizer->AddSpacer(15);
	{
		wxSizer* symbolSizer = new wxBoxSizer(wxHORIZONTAL);
		symbolSizer->Add(new wxStaticText(this, wxID_ANY, "Complex File"), 0, wxLEFT | wxRIGHT, 5);

		m_symbolPath = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(225, -1), wxTE_READONLY);
		m_symbolPath->SetValue(Context::Instance()->dlgPath);
		symbolSizer->Add(m_symbolPath, 0, wxLEFT | wxRIGHT, 5);

		wxButton* btn = new wxButton(this, wxID_ANY, "...", wxDefaultPosition, wxSize(25, 25));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CodeSettingDlg::onSelectSymbolDir));
		symbolSizer->Add(btn, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(symbolSizer);
	}
	sizer->AddSpacer(15);
	for (size_t i = 0; i < NUM; ++i)
	{
		initPackerSet(sizer, i);
		sizer->AddSpacer(10);
	}
	SetSizer(sizer);

	sizer->Layout();
	Refresh();
}

void CodeSettingDlg::initPackerSet(wxSizer* sizer, int index)
{
	wxSizer* packerSizer = new wxBoxSizer(wxHORIZONTAL);
	packerSizer->Add(new wxStaticText(this, wxID_ANY, "Texture "+wxString::FromDouble(index+1)+": "), 0, wxLEFT | wxRIGHT, 5);

	m_packerPath[index] = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(225, -1), wxTE_READONLY);
	if (Context::Instance()->texMgr.textures[index])
		m_packerPath[index]->SetValue(Context::Instance()->texMgr.textures[0]->filepath);
	packerSizer->Add(m_packerPath[index], 0, wxLEFT | wxRIGHT, 5);

	wxButton* btn = new wxButton(this, wxID_ANY, "...", wxDefaultPosition, wxSize(25, 25));
	if (index == 0)
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CodeSettingDlg::onSelectPackerFile1));
	else if (index == 1)
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CodeSettingDlg::onSelectPackerFile2));
	else if (index == 2)
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CodeSettingDlg::onSelectPackerFile3));
	else if (index == 3)
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CodeSettingDlg::onSelectPackerFile4));
	packerSizer->Add(btn, 0, wxLEFT | wxRIGHT, 5);

	sizer->Add(packerSizer);
}

void CodeSettingDlg::onSetScale(wxSpinEvent& event)
{
	Context::Instance()->scale = event.GetValue() * 0.01f;
}

class DirTraverser : public wxDirTraverser
{
public:
	DirTraverser(wxArrayString& files) 
		: m_files(files) {}

	virtual wxDirTraverseResult OnFile(const wxString& filename)
	{
		m_files.Add(filename);
		return wxDIR_CONTINUE;
	}

	virtual wxDirTraverseResult OnDir(const wxString& dirname)
	{
		return wxDIR_IGNORE;

// 		if (wxFileName(dirname).GetFullName() == wxT(".svn"))
// 			return wxDIR_IGNORE;
// 		else
// 			return wxDIR_CONTINUE;
	}

private:
	wxArrayString& m_files;

}; // DirTraverser

void CodeSettingDlg::onSelectSymbolFile(wxCommandEvent& event)
{
}

void CodeSettingDlg::onSelectSymbolDir(wxCommandEvent& event)
{
//  	wxFileDialog dlg(this, wxT("Complex symbol file"), wxEmptyString, wxEmptyString, 
//  		wxT("*_complex.json"), wxFD_OPEN);
//  	if (dlg.ShowModal() == wxID_OK)
//  	{
//  		std::string path = dlg.GetPath();
//  		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(path);
//  		Context::Instance()->symbols.push_back(static_cast<d2d::ComplexSymbol*>(symbol));
//  //		if (Context::Instance()->symbol)
//  			m_symbolPath->SetValue(path);
//  	}

	//////////////////////////////////////////////////////////////////////////

  	wxDirDialog dlg(NULL, "Complex symbol file", wxEmptyString,
  		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
  	if (dlg.ShowModal() == wxID_OK)
  	{
  		wxArrayString files;
  		DirTraverser traverser(files);
  
  		wxDir dir(dlg.GetPath());
  		dir.Traverse(traverser);
  
  		Context::Instance()->dlgPath = dlg.GetPath();
  		m_symbolPath->SetValue(dlg.GetPath());
  
  		d2d::ComplexSymbol* tot = new d2d::ComplexSymbol();
  		for (size_t i = 0, n = files.size(); i < n; ++i)
  		{
  			wxString filepath = files[i];
  			if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex))
  			{
  				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(filepath);
  				Context::Instance()->symbols.push_back(static_cast<d2d::ComplexSymbol*>(symbol));
  			}
  			else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
  			{
  				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(filepath);
  
  				d2d::ISprite* child = d2d::SpriteFactory::Instance()->create(symbol);
  				tot->m_sprites.push_back(child);
  			}
  		}
  		if (!tot->m_sprites.empty())
  			Context::Instance()->symbols.push_back(tot);
  	}
}

void CodeSettingDlg::onSelectPackerFile1(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Texture Packer file"), wxEmptyString, wxEmptyString, 
		wxT("*.json"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string path = dlg.GetPath();
		if (Context::Instance()->texMgr.initTexture(path, 0))
			m_packerPath[0]->SetValue(path);
	}
}

void CodeSettingDlg::onSelectPackerFile2(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Texture Packer file"), wxEmptyString, wxEmptyString, 
		wxT("*.json"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string path = dlg.GetPath();
		if (Context::Instance()->texMgr.initTexture(path, 1))
			m_packerPath[1]->SetValue(path);
	}
}

void CodeSettingDlg::onSelectPackerFile3(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Texture Packer file"), wxEmptyString, wxEmptyString, 
		wxT("*.json"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string path = dlg.GetPath();
		if (Context::Instance()->texMgr.initTexture(path, 2))
			m_packerPath[2]->SetValue(path);
	}
}

void CodeSettingDlg::onSelectPackerFile4(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Texture Packer file"), wxEmptyString, wxEmptyString, 
		wxT("*.json"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string path = dlg.GetPath();
		if (Context::Instance()->texMgr.initTexture(path, 3))
			m_packerPath[3]->SetValue(path);	
	}
}
} // coceditor