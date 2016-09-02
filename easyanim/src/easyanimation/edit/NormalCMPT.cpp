#include "NormalCMPT.h"
#include "ArrangeSpriteOP.h"

#include "view/ViewMgr.h"
#include "view/StagePanel.h"
#include "view/KeysPanel.h"
#include "frame/FileIO.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "dataset/DataMgr.h"
#include "message/messages.h"

#include <ee/LibraryPanel.h>
#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/LibraryList.h>
#include <ee/LibraryPage.h>
#include <ee/FetchAllVisitor.h>

#include <wx/spinctrl.h>

namespace eanim
{

NormalCMPT::NormalCMPT(wxWindow* parent, const std::string& name, bool vertical)
	: ee::EditCMPT(parent, name, ViewMgr::Instance()->stage->GetStageImpl())
	, m_vertical(vertical)
{
	m_editop = new ArrangeSpriteOP(ViewMgr::Instance()->stage);
}

wxSizer* NormalCMPT::InitLayout()
{
	return InitEditPanel();
}

wxSizer* NormalCMPT::InitEditPanel()
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

wxSizer* NormalCMPT::InitLoadPanel()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	// folder
	{
		wxButton* btnLoad = new wxButton(this, wxID_ANY, wxT("Load Folder"));
		Connect(btnLoad->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(NormalCMPT::OnLoadFromFolder));
		sizer->Add(btnLoad);
	}
	sizer->AddSpacer(10);
	// all image
	{
		wxButton* btnLoad = new wxButton(this, wxID_ANY, wxT("Load List"));
		Connect(btnLoad->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(NormalCMPT::OnLoadFromList));
		sizer->Add(btnLoad);
	}
	return sizer;
}

wxSizer* NormalCMPT::InitFillingPanel()
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
			wxCommandEventHandler(NormalCMPT::OnFillingFrames));
		filling_sizer->Add(btn);
	}
	return filling_sizer;
}

wxSizer* NormalCMPT::InitSettingsPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("¸¨ÖúÏß"));
	int orient = m_vertical ? wxVERTICAL : wxHORIZONTAL;
	wxSizer* sizer = new wxStaticBoxSizer(bounding, orient);

	wxButton* btnAdd = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(25, 25));
	sizer->Add(btnAdd, 0, wxLEFT | wxRIGHT, 5);
	Connect(btnAdd->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(NormalCMPT::OnAddCross));
	wxButton* btnDel = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(25, 25));
	sizer->Add(btnDel, 0, wxLEFT | wxRIGHT, 5);
	Connect(btnDel->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(NormalCMPT::OnDelCross));

	return sizer;
}

void NormalCMPT::OnLoadFromFolder(wxCommandEvent& event)
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
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(itr->second[i]);
//			sym->refresh();
			ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
			frame->Insert(spr, INT_MAX);
			spr->RemoveReference();
			sym->RemoveReference();
		}
		layer->InsertKeyFrame(frame);
		frame->RemoveReference();
	}
	InsertLayerSJ::Instance()->Insert(layer);
	SetSelectedSJ::Instance()->Set(0, 0);

	ViewMgr::Instance()->library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

void NormalCMPT::OnLoadFromList(wxCommandEvent& event)
{
	std::vector<ee::ListItem*> items;
	ViewMgr::Instance()->img_page->GetList()->
		Traverse(ee::FetchAllVisitor<ee::ListItem>(items));
	if (items.empty()) {
		return;
	}

	Layer* layer = new Layer;
	DataMgr::Instance()->GetLayers().Clear();
	SetSelectedSJ::Instance()->Set(-1, -1);
	InsertLayerSJ::Instance()->Insert(layer);
	SetSelectedSJ::Instance()->Set(0, 0);

	int tot = m_filling->GetValue() - 1;
	int space = tot / (items.size() - 1);
	int frame_idx = 1;
	for (size_t i = 0, n = items.size(); i < n; ++i)
	{
		ee::Symbol* sym = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(items[i]));
		KeyFrame* frame = new KeyFrame(frame_idx);
		ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);

		frame->Insert(spr, INT_MAX);
		layer->InsertKeyFrame(frame);

		spr->RemoveReference();
		frame->RemoveReference();

		frame_idx += space;
	}	

	SetSelectedSJ::Instance()->Set(0, 0);
}

void NormalCMPT::OnFillingFrames(wxCommandEvent& event)
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
			itr->second->AddReference();
			fixed.push_back(itr->second);
		}

		layer->Clear();
		for (size_t i = 0, n = fixed.size(); i < n; ++i) {
			layer->InsertKeyFrame(fixed[i]);
			fixed[i]->RemoveReference();
		}
	}
}

void NormalCMPT::OnChangeAnim(wxCommandEvent& event)
{
	DataMgr::Instance()->GetTemplate().SetChoice(event.GetInt());
	FileIO::Reload();
}

void NormalCMPT::OnAddCross(wxCommandEvent& event)
{
	static_cast<ArrangeSpriteOP*>(m_editop)->AddCross();

}

void NormalCMPT::OnDelCross(wxCommandEvent& event)
{
	static_cast<ArrangeSpriteOP*>(m_editop)->DelCross();
}

}