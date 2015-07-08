#include "RectCutCMPT.h"
#include "RectCutOP.h"
#include "StagePanel.h"

#include <easycomplex.h>
#include <easyimage.h>

namespace eimage
{

static const std::string FILTER = "rectcut";

RectCutCMPT::RectCutCMPT(wxWindow* parent, const wxString& name,
						 StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_stage(stage)
{
	m_editOP = new RectCutOP(this, stage);
}

void RectCutCMPT::onSaveEditOP(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILTER + wxT(".json"), wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
		RectCutOP* op = static_cast<RectCutOP*>(m_editOP);

		Json::Value value;

		std::string filepath = op->getImageFilepath();
		std::string dir = d2d::FilenameTools::getFileDir(dlg.GetPath());
		value["image filepath"] = d2d::FilenameTools::getRelativePath(dir, filepath).ToStdString();
		op->getRectMgr().store(value);

		wxString filename = d2d::FilenameTools::getFilenameAddTag(dlg.GetPath(), FILTER, "json");
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filename.fn_str());
		std::locale::global(std::locale("C"));
		writer.write(fout, value);
		fout.close();
	}
}

void RectCutCMPT::onLoadEditOP(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILTER + wxT(".json"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString filename = d2d::FilenameTools::getFilenameAddTag(dlg.GetPath(), FILTER, "json");
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filename.fn_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		RectCutOP* op = static_cast<RectCutOP*>(m_editOP);
		wxString dlgpath = d2d::FilenameTools::getFileDir(filename);
		wxString path = value["image filepath"].asString();
		wxString absolutePath = d2d::FilenameTools::getAbsolutePath(dlgpath, path);
		op->loadImageFromFile(absolutePath.ToStdString());
		op->getRectMgr().load(value);
	}
}

wxSizer* RectCutCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
// 	sizer->Add(initEditIOLayout());
// 	sizer->AddSpacer(10);
	sizer->Add(initDataOutputLayout());
	sizer->AddSpacer(10);
	sizer->Add(initAddRectLayout());
	return sizer;
}

wxSizer* RectCutCMPT::initEditIOLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Edit I/O"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
	// save
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Save"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::onSaveEditOP));
		sizer->Add(btn);
	}
	sizer->AddSpacer(5);
	// load
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Load"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::onLoadEditOP));
		sizer->Add(btn);
	}
	return sizer;
}

wxSizer* RectCutCMPT::initDataOutputLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Data Output"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	// images path
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("images path"));
		wxSizer* sz = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		m_imagePath = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY);
		sz->Add(m_imagePath);

		sz->AddSpacer(5);

		wxButton* btn = new wxButton(this, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::onSetImagesPath));
		sz->Add(btn);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	// json path
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("json path"));
		wxSizer* sz = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		m_jsonPath = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY);
		sz->Add(m_jsonPath);

		sz->AddSpacer(5);

		wxButton* btn = new wxButton(this, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::onSetJsonPath));
		sz->Add(btn);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	// output
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Output"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::onOutputData));
		sizer->Add(btn);
	}
	return sizer;
}

wxSizer* RectCutCMPT::initAddRectLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Add Rect"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);

		m_widthCtrl = new wxTextCtrl(this, wxID_ANY, "100", wxDefaultPosition, wxSize(60, -1));
		inputSizer->Add(m_widthCtrl);

		inputSizer->AddSpacer(5);

		m_heightCtrl = new wxTextCtrl(this, wxID_ANY, "100", wxDefaultPosition, wxSize(60, -1));
		inputSizer->Add(m_heightCtrl);

		sizer->Add(inputSizer);
	}
	sizer->AddSpacer(5);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Add"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::onAddRect));
		sizer->Add(btn);
	}
	return sizer;
}

void RectCutCMPT::onSetImagesPath(wxCommandEvent& event)
{
	RectCutOP* op = static_cast<RectCutOP*>(m_editOP);
	op->setMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Images Path", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_imagePath->SetValue(dlg.GetPath());
	}

	op->setMouseMoveFocus(true);
}

void RectCutCMPT::onSetJsonPath(wxCommandEvent& event)
{
	RectCutOP* op = static_cast<RectCutOP*>(m_editOP);
	op->setMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Json Path", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_jsonPath->SetValue(dlg.GetPath());
	}

	op->setMouseMoveFocus(true);
}

void RectCutCMPT::onOutputData(wxCommandEvent& event)
{
	RectCutOP* op = static_cast<RectCutOP*>(m_editOP);
	const std::vector<d2d::Rect*>& rects = op->getRectMgr().getAllRect();
	if (rects.empty()) {
		return;
	}

	const d2d::ISprite* sprite = m_stage->getImage();
	if (!sprite) {
		return;
	}

	const d2d::ImageSprite* imgSprite = dynamic_cast<const d2d::ImageSprite*>(sprite);
	if (!imgSprite) {
		return;
	}

	d2d::Image* image = imgSprite->getSymbol().getImage();

	wxString imageDir = m_imagePath->GetValue();
	wxString jsonDir = m_jsonPath->GetValue();

	wxString imageName = d2d::FilenameTools::getFilename(image->GetFilepath());
	ecomplex::Symbol* complex = new ecomplex::Symbol;
	for (int i = 0, n = rects.size(); i < n; ++i)
	{
		const d2d::Rect& r = *rects[i];

		eimage::ImageClip clip(image);
		const uint8_t* pixels = clip.Clip(r.xMin, r.xMax, r.yMin, r.yMax);
		float width = r.xLength();
		float height = r.yLength();

		wxString img_filename = imageDir + "\\" + imageName + "_" + wxString::FromDouble(i);
		d2d::ImageSaver::storeToFile(pixels, width, height, 4, img_filename.ToStdString(), d2d::ImageSaver::e_png);

		wxString img_fullname = img_filename + ".png";
		d2d::ISprite* sprite = new d2d::NullSprite(new d2d::NullSymbol(img_fullname.ToStdString(), width, height));
		d2d::Vector off;
		off.x = r.xCenter() - image->GetClippedWidth() * 0.5f;
		off.y = r.yCenter() - image->GetClippedHeight() * 0.5f;
		sprite->translate(off);
		complex->m_sprites.push_back(sprite);
	}

	wxString tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_complex);
	wxString json_filename = jsonDir + "\\" + imageName + "_" + tag + ".json";
	ecomplex::FileSaver::store(json_filename.c_str(), complex);
	delete complex;

	d2d::FinishDialog dlg(this);
	dlg.ShowModal();
}

void RectCutCMPT::onAddRect(wxCommandEvent& event)
{
	double width, height;
	m_widthCtrl->GetValue().ToDouble(&width);
	m_heightCtrl->GetValue().ToDouble(&height);
	
	RectCutOP* op = static_cast<RectCutOP*>(m_editOP);
	op->getRectMgr().insert(d2d::Rect(d2d::Vector(0, 0), d2d::Vector((float)width, (float)height)));

	m_stage->RefreshStage();
}

}