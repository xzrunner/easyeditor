#include "CommonCMPT.h"
#include "ArrangeSpriteOP.h"

#include "view/ViewMgr.h"
#include "view/StagePanel.h"
#include "view/KeysPanel.h"
#include "frame/FileIO.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "dataset/DataMgr.h"
#include "message/messages.h"

namespace eanim
{

CommonCMPT::CommonCMPT(wxWindow* parent, const std::string& name, bool vertical)
	: ee::EditCMPT(parent, name, ViewMgr::Instance()->stage->GetStageImpl())
	, m_vertical(vertical)
{
	m_editop = new ArrangeSpriteOP(ViewMgr::Instance()->stage);
}

wxSizer* CommonCMPT::InitLayout()
{
	return InitEditPanel();
}

wxSizer* CommonCMPT::InitEditPanel()
{
	int orient = m_vertical ? wxVERTICAL : wxHORIZONTAL;
	wxBoxSizer* sizer = new wxBoxSizer(orient);
	sizer->AddSpacer(10);
	sizer->Add(InitLoadPanel());
	sizer->AddSpacer(20);
	sizer->Add(InitFillingPanel());
	sizer->AddSpacer(10);
	sizer->Add(InitSettingsPanel());

	return sizer;
}

wxSizer* CommonCMPT::InitLoadPanel()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	// folder
	{
		wxButton* btnLoad = new wxButton(this, wxID_ANY, wxT("Load Folder"));
		Connect(btnLoad->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CommonCMPT::OnLoadFromFolder));
		sizer->Add(btnLoad);
	}
	sizer->AddSpacer(10);
	// all image
	{
		wxButton* btnLoad = new wxButton(this, wxID_ANY, wxT("Load List"));
		Connect(btnLoad->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CommonCMPT::OnLoadFromList));
		sizer->Add(btnLoad);
	}
	return sizer;
}

wxSizer* CommonCMPT::InitFillingPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Filling"));
	int orient = m_vertical ? wxVERTICAL : wxHORIZONTAL;
	wxSizer* filling_sizer = new wxStaticBoxSizer(bounding, orient);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("tot frames: ")));

		m_filling = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
			wxSP_ARROW_KEYS, 10, 1000, 0);
		sizer->Add(m_filling);

		filling_sizer->Add(sizer);
	}
	filling_sizer->AddSpacer(5);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Filling"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CommonCMPT::OnFillingFrames));
		filling_sizer->Add(btn);
	}
	return filling_sizer;
}

wxSizer* CommonCMPT::InitSettingsPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("¸¨ÖúÏß"));
	int orient = m_vertical ? wxVERTICAL : wxHORIZONTAL;
	wxSizer* sizer = new wxStaticBoxSizer(bounding, orient);

	wxButton* btnAdd = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(25, 25));
	sizer->Add(btnAdd, 0, wxLEFT | wxRIGHT, 5);
	Connect(btnAdd->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(CommonCMPT::OnAddCross));
	wxButton* btnDel = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(25, 25));
	sizer->Add(btnDel, 0, wxLEFT | wxRIGHT, 5);
	Connect(btnDel->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(CommonCMPT::OnDelCross));

	return sizer;
}

void CommonCMPT::OnLoadFromFolder(wxCommandEvent& event)
{
	ArrangeSpriteOP* op = static_cast<ArrangeSpriteOP*>(m_editop);
	op->SetMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Images", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() != wxID_OK)
		return;

	DataMgr::Instance()->GetLayers().Clear();
	ViewMgr::Instance()->library->Clear();
	SetSelectedSJ::Instance()->Set(-1, -1);

	op->SetMouseMoveFocus(true);

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dlg.GetPath().ToStdString(), files);

	std::map<int, std::vector<std::string> > mapFrameSymbols;
	for (size_t i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = files[i];
		if (!ee::FileType::IsType(filepath, ee::FileType::e_image))
			continue;

		std::string name = ee::FileHelper::GetFilename(filepath);
		size_t mid = name.find('_');
		if (mid == std::string::npos)
			continue;

		int item = ee::StringHelper::FromString<int>(name.substr(0, mid)),
			frame = ee::StringHelper::FromString<int>(name.substr(mid+1));		
		std::map<int, std::vector<std::string> >::iterator itr 
			= mapFrameSymbols.find(frame);
		if (itr == mapFrameSymbols.end())
		{
			std::vector<std::string> items;
			items.push_back(filepath);
			mapFrameSymbols.insert(std::make_pair(frame, items));
		}
		else
		{
			itr->second.push_back(filepath);
		}
	}

	DataMgr::Instance()->GetLayers().Clear();
	ViewMgr::Instance()->library->Clear();
	SetSelectedSJ::Instance()->Set(-1, -1);

	Layer* layer = new Layer();
	std::map<int, std::vector<std::string> >::iterator itr
		= mapFrameSymbols.begin();
	for ( ; itr != mapFrameSymbols.end(); ++itr)
	{
		KeyFrame* frame = new KeyFrame(itr->first);
		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(itr->second[i]);
//			symbol->refresh();
			ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
			frame->Insert(sprite);
			sprite->Release();
			symbol->Release();
		}
		layer->InsertKeyFrame(frame);
		frame->Release();
	}
	InsertLayerSJ::Instance()->Insert(layer);
	SetSelectedSJ::Instance()->Set(0, 0);

	ViewMgr::Instance()->library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

void CommonCMPT::OnLoadFromList(wxCommandEvent& event)
{
	std::vector<ee::Symbol*> symbols;
	ViewMgr::Instance()->img_page->GetList()->
		Traverse(ee::FetchAllVisitor<ee::Symbol>(symbols));
	if (symbols.empty()) {
		return;
	}

	Layer* layer = new Layer;
	DataMgr::Instance()->GetLayers().Clear();
	SetSelectedSJ::Instance()->Set(-1, -1);
	InsertLayerSJ::Instance()->Insert(layer);
	SetSelectedSJ::Instance()->Set(0, 0);

	int tot = m_filling->GetValue() - 1;
	int space = tot / (symbols.size() - 1);
	int frame_idx = 1;
	for (size_t i = 0, n = symbols.size(); i < n; ++i)
	{
		KeyFrame* frame = new KeyFrame(frame_idx);
		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbols[i]);

		frame->Insert(sprite);		
		layer->InsertKeyFrame(frame);

		sprite->Release();
		frame->Release();

		frame_idx += space;
	}	

	SetSelectedSJ::Instance()->Set(0, 0);
}

void CommonCMPT::OnFillingFrames(wxCommandEvent& event)
{
	int tot = m_filling->GetValue() - 1;
	for (size_t i = 0, n = DataMgr::Instance()->GetLayers().Size(); i < n; ++i)
	{
		Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(i);

		const std::map<int, KeyFrame*>& frames = layer->GetAllFrames();
		std::vector<KeyFrame*> fixed;
		fixed.reserve(frames.size());

		int dis = tot / (frames.size() - 1);
		std::map<int, KeyFrame*>::const_iterator itr = frames.begin();
		for (size_t i = 0; itr != frames.end(); ++itr, ++i)
		{
			itr->second->SetTime(1+dis*i);
			itr->second->Retain();
			fixed.push_back(itr->second);
		}

		layer->Clear();
		for (size_t i = 0, n = fixed.size(); i < n; ++i) {
			layer->InsertKeyFrame(fixed[i]);
			fixed[i]->Release();
		}
	}
}

void CommonCMPT::OnChangeAnim(wxCommandEvent& event)
{
	DataMgr::Instance()->GetTemplate().SetChoice(event.GetInt());
	FileIO::Reload();
}

void CommonCMPT::OnAddCross(wxCommandEvent& event)
{
	static_cast<ArrangeSpriteOP*>(m_editop)->AddCross();

}

void CommonCMPT::OnDelCross(wxCommandEvent& event)
{
	static_cast<ArrangeSpriteOP*>(m_editop)->DelCross();
}

}