#include "RectCutCMPT.h"
#include "RectCutOP.h"
#include "StagePanel.h"

#include <easycomplex.h>

namespace eimage
{

RectCutCMPT::RectCutCMPT(wxWindow* parent, const wxString& name,
						   StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_stage(stage)
{
	m_editOP = new RectCutOP(stage);
}

wxSizer* RectCutCMPT::initLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	// images path
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("images path"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		m_imagePath = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY);
		sizer->Add(m_imagePath);

		sizer->AddSpacer(5);
		
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::onSetImagesPath));
		sizer->Add(btn);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// json path
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("json path"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		m_jsonPath = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY);
		sizer->Add(m_jsonPath);

		sizer->AddSpacer(5);

		wxButton* btn = new wxButton(this, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::onSetJsonPath));
		sizer->Add(btn);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// output
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Output"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::onOutputData));
		topSizer->Add(btn);
	}

	return topSizer;
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
	const std::vector<d2d::Rect*>& rects = op->getAllRect();
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

	wxString imageName = d2d::FilenameTools::getFilename(image->filepath());
	ecomplex::Symbol* complex = new ecomplex::Symbol;
	for (int i = 0, n = rects.size(); i < n; ++i)
	{
		const d2d::Rect& r = *rects[i];
		const unsigned char* pixels = image->clip(r.xMin, r.xMax, r.yMin, r.yMax);

		float width = r.xLength();
		float height = r.yLength();

		wxString img_filename = imageDir + "\\" + imageName + "_" + wxString::FromDouble(i);
		d2d::ImageSaver::storeToFile(pixels, width, height, img_filename.ToStdString(), d2d::ImageSaver::e_png);

		wxString img_fullname = img_filename + ".png";
		d2d::ISprite* sprite = new d2d::NullSprite(new d2d::NullSymbol(img_fullname.ToStdString(), width, height));
		d2d::Vector off;
		off.x = r.xCenter() - image->width() * 0.5f;
		off.y = r.yCenter() - image->height() * 0.5f;
		sprite->translate(off);
		complex->m_sprites.push_back(sprite);
	}

	wxString tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_complex);
	wxString json_filename = jsonDir + "\\" + imageName + "_" + tag + ".json";
	ecomplex::FileSaver::store(json_filename.c_str(), complex);
	delete complex;
}

}