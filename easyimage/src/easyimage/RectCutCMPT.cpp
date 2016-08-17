#include "RectCutCMPT.h"
#include "RectCutOP.h"
#include "StagePanel.h"

#include <ee/FileHelper.h>
#include <ee/ImageSprite.h>
#include <ee/Image.h>
#include <ee/StringHelper.h>
#include <ee/panel_msg.h>
#include <ee/ImageSaver.h>
#include <ee/DummySprite.h>
#include <ee/DummySymbol.h>
#include <ee/FinishDialog.h>
#include <ee/ImageClip.h>

#include <easycomplex.h>
#include <easyimage.h>

#include <fstream>

namespace eimage
{

static const std::string FILTER = "rectcut";

RectCutCMPT::RectCutCMPT(wxWindow* parent, const std::string& name,
						 StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage(stage)
{
	m_editop = new RectCutOP(this, stage);
}

void RectCutCMPT::OnSaveEditOP(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILTER + wxT(".json"), wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
		RectCutOP* op = static_cast<RectCutOP*>(m_editop);

		Json::Value value;

		std::string filepath = op->GetImageFilepath();
		std::string dir = ee::FileHelper::GetFileDir(dlg.GetPath().ToStdString());
		value["image filepath"] = ee::FileHelper::GetRelativePath(dir, filepath);
		op->GetRectMgr().Store(value);

		const sm::vec2& center = op->GetCenter();
		value["center"]["x"] = center.x;
		value["center"]["y"] = center.y;

		for (int i = 0, n = m_part_rects.size(); i < n; ++i) {
			const sm::rect& r = m_part_rects[i];
			value["part_rect"][i]["xmin"] = r.xmin;
			value["part_rect"][i]["xmax"] = r.xmax;
			value["part_rect"][i]["ymin"] = r.ymin;
			value["part_rect"][i]["ymax"] = r.ymax;
		}

		std::string filename = ee::FileHelper::GetFilenameAddTag(dlg.GetPath().ToStdString(), FILTER, "json");
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filename.c_str());
		std::locale::global(std::locale("C"));
		writer.write(fout, value);
		fout.close();
	}
}

void RectCutCMPT::OnLoadEditOP(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILTER + wxT(".json"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string filename = ee::FileHelper::GetFilenameAddTag(dlg.GetPath().ToStdString(), FILTER, "json");
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filename.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		RectCutOP* op = static_cast<RectCutOP*>(m_editop);

		sm::vec2 center;
		center.x = value["center"]["x"].asDouble();
		center.y = value["center"]["y"].asDouble();
		op->SetCenter(center);

		int i = 0;
		Json::Value val = value["part_rect"][i++];
		while (!val.isNull()) {
			sm::rect r;
			r.xmin = static_cast<float>(val["xmin"].asDouble());
			r.xmax = static_cast<float>(val["xmax"].asDouble());
			r.ymin = static_cast<float>(val["ymin"].asDouble());
			r.ymax = static_cast<float>(val["ymax"].asDouble());
			m_part_rects.push_back(r);
			val = value["part_rect"][i++];
		}

		std::string dlgpath = ee::FileHelper::GetFileDir(filename);
		std::string path = value["image filepath"].asString();
		std::string absolutePath = ee::FileHelper::GetAbsolutePath(dlgpath, path);
		op->LoadImageFromFile(absolutePath);

		op->GetRectMgr().Load(value);
	}
}

wxSizer* RectCutCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
// 	sizer->Add(initEditIOLayout());
// 	sizer->AddSpacer(10);
	sizer->Add(InitDataOutputLayout());
	sizer->AddSpacer(10);
	sizer->Add(InitAddRectLayout());
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Auto"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnAutoCreateRects));
		sizer->Add(btn);
	}
	return sizer;
}

wxSizer* RectCutCMPT::InitEditIOLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Edit I/O"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
	// save
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Save"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnSaveEditOP));
		sizer->Add(btn);
	}
	sizer->AddSpacer(5);
	// load
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Load"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnLoadEditOP));
		sizer->Add(btn);
	}
	return sizer;
}

wxSizer* RectCutCMPT::InitDataOutputLayout()
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
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnSetImagesPath));
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
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnSetJsonPath));
		sz->Add(btn);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	// output
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Output"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnOutputData));
		sizer->Add(btn);
	}
	return sizer;
}

wxSizer* RectCutCMPT::InitAddRectLayout()
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
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnAddRect));
		sizer->Add(btn);
	}
	return sizer;
}

void RectCutCMPT::OnSetImagesPath(wxCommandEvent& event)
{
	RectCutOP* op = static_cast<RectCutOP*>(m_editop);
	op->SetMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Images Path", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_imagePath->SetValue(dlg.GetPath());
	}

	op->SetMouseMoveFocus(true);
}

void RectCutCMPT::OnSetJsonPath(wxCommandEvent& event)
{
	RectCutOP* op = static_cast<RectCutOP*>(m_editop);
	op->SetMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Json Path", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_jsonPath->SetValue(dlg.GetPath());
	}

	op->SetMouseMoveFocus(true);
}

void RectCutCMPT::OnOutputData(wxCommandEvent& event)
{
	RectCutOP* op = static_cast<RectCutOP*>(m_editop);
	const std::vector<sm::rect*>& rects = op->GetRectMgr().GetAllRect();
	if (rects.empty()) {
		return;
	}

	const ee::Sprite* spr = m_stage->GetImage();
	if (!spr) {
		return;
	}

	const ee::ImageSprite* img_spr = dynamic_cast<const ee::ImageSprite*>(spr);
	if (!img_spr) {
		return;
	}

	ee::Image* image = img_spr->GetSymbol()->GetImage();

	std::string img_dir = m_imagePath->GetValue();
	std::string json_dir = m_jsonPath->GetValue();

	
	sm::vec2 center = op->GetCenter();
	if (center == sm::vec2(0, 0)) {
		center.x = image->GetClippedWidth() * 0.5f;
		center.y = image->GetClippedHeight() * 0.5f;
	}

	std::string img_name = ee::FileHelper::GetFilename(image->GetFilepath());
	ecomplex::Symbol* complex_all = new ecomplex::Symbol;
	ecomplex::Symbol* complex_part = new ecomplex::Symbol;
	for (int i = 0, n = rects.size(); i < n; ++i)
	{
		const sm::rect& r = *rects[i];

		ee::ImageClip clip(*image->GetImageData());
		const uint8_t* pixels = clip.Clip(r.xmin, r.xmax, r.ymin, r.ymax);
		sm::vec2 sz = r.Size();

		std::string img_filename = img_dir + "\\" + img_name + "_" + ee::StringHelper::ToString(i) + ".png";
		ee::ImageSaver::StoreToFile(pixels, sz.x, sz.y, 4, img_filename, ee::ImageSaver::e_png);

		ee::Sprite* spr = new ee::DummySprite(new ee::DummySymbol(img_filename, sz.x, sz.y));
		sm::vec2 offset = r.Center() - center;
		spr->Translate(offset);
		complex_all->Add(spr);

		for (int j = 0, m = m_part_rects.size(); j < m; ++j) {
			if (m_part_rects[j] == r) {
				spr->AddReference();
				complex_part->Add(spr);
				break;
			}
		}
	}
	complex_all->name = img_name;
	complex_part->name = img_name + "_part";

	std::string tag = ee::FileType::GetTag(ee::FileType::e_complex);

	std::string filename_all = json_dir + "\\" + img_name + "_" + tag + ".json";
	ecomplex::FileStorer::Store(filename_all.c_str(), complex_all);
	delete complex_all;

	if (!complex_part->GetChildren().empty()) {
		std::string filename_part = json_dir + "\\" + img_name + "_part_" + tag + ".json";
		ecomplex::FileStorer::Store(filename_part.c_str(), complex_part);
	}
	delete complex_part;

	ee::FinishDialog dlg(this);
	dlg.ShowModal();
}

void RectCutCMPT::OnAddRect(wxCommandEvent& event)
{
	double width, height;
	m_widthCtrl->GetValue().ToDouble(&width);
	m_heightCtrl->GetValue().ToDouble(&height);
	
	RectCutOP* op = static_cast<RectCutOP*>(m_editop);
	op->GetRectMgr().Insert(sm::rect(sm::vec2(0, 0), sm::vec2((float)width, (float)height)));

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void RectCutCMPT::OnAutoCreateRects(wxCommandEvent& event)
{
	const ee::Sprite* spr = m_stage->GetImage();
	const ee::ImageSprite* img_sprite 
		= dynamic_cast<const ee::ImageSprite*>(spr);
	assert(img_sprite);
	const ee::Image* img = img_sprite->GetSymbol()->GetImage();

	RectMgr& rects = static_cast<RectCutOP*>(m_editop)->GetRectMgr();

	m_part_rects.clear();
	std::vector<Rect> pre_rects;
	const std::vector<sm::rect*>& _rects = rects.GetAllRect();
	for (int i = 0, n = _rects.size(); i < n; ++i) {
		const sm::rect& src = *_rects[i];
		m_part_rects.push_back(src);
		Rect dst;
		dst.x = src.xmin;
		dst.y = src.ymin;
		dst.w = src.xmax - src.xmin;
		dst.h = src.ymax - src.ymin;
		dst.is_const = true;
		pre_rects.push_back(dst);
	}
	rects.Clear();

	RegularRectCut cut(*img, pre_rects);
	cut.AutoCut();

	const std::vector<Rect>& result = cut.GetResult();
	for (int i = 0, n = result.size(); i < n; ++i) {
		int x = result[i].x,
			y = result[i].y,
			w = result[i].w,
			h = result[i].h;
		rects.Insert(sm::rect(sm::vec2(x, y), sm::vec2(x+w, y+h)));
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	std::string msg = ee::StringHelper::Format("Left: %d, Used: %d", cut.GetLeftArea(), cut.GetUseArea());
	wxMessageBox(msg, wxT("Statics"), wxOK | wxICON_INFORMATION, this);
}

}