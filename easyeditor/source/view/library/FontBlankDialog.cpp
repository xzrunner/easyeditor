#include "FontBlankDialog.h"
#include "FontBlankSymbol.h"
#include "SymbolFile.h"
#include "FileHelper.h"
#include "StringHelper.h"
#include "SymbolType.h"

#include <json/json.h>

#include <fstream>

namespace ee
{

FontBlankDialog::FontBlankDialog(wxWindow* parent, FontBlankSymbol* sym)
 		: wxDialog(parent, wxID_ANY, "Edit Font", wxDefaultPosition, 
		wxSize(400, 300), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
		, m_sym(sym)
{
	InitLayout();
}

FontBlankDialog::~FontBlankDialog()
{
	m_sym->name = m_name->GetValue();
	m_sym->font = m_font->GetValue();
	m_sym->color = m_color->GetValue();

	double tmp;
	m_align->GetValue().ToDouble(&tmp);
	m_sym->align_hori = tmp;

	m_size->GetValue().ToDouble(&tmp);
	m_sym->size = tmp;

	m_width->GetValue().ToDouble(&tmp);
	m_sym->width = tmp;

	m_height->GetValue().ToDouble(&tmp);
	m_sym->height = tmp;

	StoreSymbol();
}

void FontBlankDialog::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "name"), 0, wxLEFT | wxRIGHT, 5);

		m_name = new wxTextCtrl(this, wxID_ANY);
		m_name->SetValue(m_sym->name);
		csizer->Add(m_name, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "font"), 0, wxLEFT | wxRIGHT, 5);

		m_font = new wxTextCtrl(this, wxID_ANY);
		m_font->SetValue(m_sym->font);
		csizer->Add(m_font, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "color"), 0, wxLEFT | wxRIGHT, 5);

		m_color = new wxTextCtrl(this, wxID_ANY);
		m_color->SetValue(m_sym->color);
		csizer->Add(m_color, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "align"), 0, wxLEFT | wxRIGHT, 5);

		m_align = new wxTextCtrl(this, wxID_ANY);
		m_align->SetValue(StringHelper::ToString(m_sym->align_hori));
		csizer->Add(m_align, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "size"), 0, wxLEFT | wxRIGHT, 5);

		m_size = new wxTextCtrl(this, wxID_ANY);
		m_size->SetValue(StringHelper::ToString(m_sym->size));
		csizer->Add(m_size, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "width"), 0, wxLEFT | wxRIGHT, 5);

		m_width = new wxTextCtrl(this, wxID_ANY);
		m_width->SetValue(StringHelper::ToString(m_sym->width));
		csizer->Add(m_width, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "height"), 0, wxLEFT | wxRIGHT, 5);

		m_height = new wxTextCtrl(this, wxID_ANY);
		m_height->SetValue(StringHelper::ToString(m_sym->height));
		csizer->Add(m_height, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	SetSizer(sizer);

	sizer->Layout();
	Refresh(true);
}

void FontBlankDialog::StoreSymbol()
{
	if (!m_sym) return;

	std::string filepath = m_sym->GetFilepath();
	if (filepath.empty())
	{
		std::string ext = ee::SymbolFile::Instance()->Tag(SYM_FONTBLANK);
		ext = "_" + ext + ".json";

		std::string name = m_sym->name;
		if (name.empty()) 
		{
			name = m_sym->GetName();
		}

		std::string dir = FileHelper::GetFileDir(m_sym->GetFilepath());
		filepath = dir + "\\" + m_sym->name + ext;
	}

	Json::Value value;

	value["name"] = m_sym->name;
	value["font"] = m_sym->font;
	value["color"] = m_sym->color;
	value["align hori"] = m_sym->align_hori;
	value["align vert"] = m_sym->align_vert;
	value["size"] = m_sym->size;
	value["width"] = m_sym->width;
	value["height"] = m_sym->height;

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();

	m_sym->LoadFromFile(filepath);
}
}