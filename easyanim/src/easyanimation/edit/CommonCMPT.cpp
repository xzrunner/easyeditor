#include "CommonCMPT.h"
#include "ArrangeSpriteOP.h"

#include "view/StagePanel.h"
#include "view/KeysPanel.h"
#include "frame/Context.h"
#include "frame/FileIO.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"

namespace eanim
{

CommonCMPT::CommonCMPT(wxWindow* parent, const wxString& name, 
	StagePanel* stage, d2d::PropertySettingPanel* property, bool vertical)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_vertical(vertical)
{
	m_editOP = new ArrangeSpriteOP(stage, property);
}

wxSizer* CommonCMPT::initLayout()
{
	return initEditPanel();
}

wxSizer* CommonCMPT::initEditPanel()
{
	int orient = m_vertical ? wxVERTICAL : wxHORIZONTAL;
	wxBoxSizer* sizer = new wxBoxSizer(orient);
	sizer->AddSpacer(10);
	sizer->Add(initLoadPanel());
	sizer->AddSpacer(20);
	sizer->Add(initFillingPanel());
	sizer->AddSpacer(10);
	sizer->Add(initSettingsPanel());

	return sizer;
}

wxSizer* CommonCMPT::initLoadPanel()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	// folder
	{
		wxButton* btnLoad = new wxButton(this, wxID_ANY, wxT("Load Folder"));
		Connect(btnLoad->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CommonCMPT::onLoadFromFolder));
		sizer->Add(btnLoad);
	}
	sizer->AddSpacer(10);
	// all image
	{
		wxButton* btnLoad = new wxButton(this, wxID_ANY, wxT("Load List"));
		Connect(btnLoad->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CommonCMPT::onLoadFromList));
		sizer->Add(btnLoad);
	}
	return sizer;
}

wxSizer* CommonCMPT::initFillingPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Filling"));
	int orient = m_vertical ? wxVERTICAL : wxHORIZONTAL;
	wxSizer* sizer = new wxStaticBoxSizer(bounding, orient);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("tot frames: ")));

		m_filling = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
			wxSP_ARROW_KEYS, 10, 1000, 0);
		sizer->Add(m_filling);

		sizer->Add(sizer);
	}
	{
		wxButton* btnFill = new wxButton(this, wxID_ANY, wxT("Filling"));
		Connect(btnFill->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CommonCMPT::onFillingFrames));
		sizer->Add(btnFill);
	}
	return sizer;
}

wxSizer* CommonCMPT::initSettingsPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("¸¨ÖúÏß"));
	int orient = m_vertical ? wxVERTICAL : wxHORIZONTAL;
	wxSizer* sizer = new wxStaticBoxSizer(bounding, orient);

	wxButton* btnAdd = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(25, 25));
	sizer->Add(btnAdd, 0, wxLEFT | wxRIGHT, 5);
	Connect(btnAdd->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(CommonCMPT::onAddCross));
	wxButton* btnDel = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(25, 25));
	sizer->Add(btnDel, 0, wxLEFT | wxRIGHT, 5);
	Connect(btnDel->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(CommonCMPT::onDelCross));

	return sizer;
}

void CommonCMPT::onLoadFromFolder(wxCommandEvent& event)
{
	ArrangeSpriteOP* op = static_cast<ArrangeSpriteOP*>(m_editOP);
	op->setMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Images", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() != wxID_OK)
		return;

	clear();

	op->setMouseMoveFocus(true);

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dlg.GetPath().ToStdString(), files);

	std::map<int, std::vector<wxString> > mapFrameSymbols;
	for (size_t i = 0, n = files.size(); i < n; ++i)
	{
		wxString filepath = files[i];
		if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
			continue;

		wxString name = d2d::FilenameTools::getFilename(filepath);
		size_t mid = name.find('_');
		if (mid == wxString::npos)
			continue;

		wxString sitem = name.substr(0, mid);
		wxString sframe = name.substr(mid+1);

		long item, frame;
		sitem.ToLong(&item);
		sframe.ToLong(&frame);
		
		std::map<int, std::vector<wxString> >::iterator itr 
			= mapFrameSymbols.find(frame);
		if (itr == mapFrameSymbols.end())
		{
			std::vector<wxString> items;
			items.push_back(filepath);
			mapFrameSymbols.insert(std::make_pair(frame, items));
		}
		else
		{
			itr->second.push_back(filepath);
		}
	}

	Context* context = Context::Instance();
	context->layers.clear();
	Layer* layer = new Layer;
	std::map<int, std::vector<wxString> >::iterator itr
		= mapFrameSymbols.begin();
	for ( ; itr != mapFrameSymbols.end(); ++itr)
	{
		KeyFrame* frame = new KeyFrame(itr->first);
		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(itr->second[i]);
//			symbol->refresh();
			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			frame->insertWithClone(sprite);
			sprite->release();
			symbol->release();
		}
		layer->insertKeyFrame(frame);
	}
	context->layers.insertLayer(layer);

	context->setCurrFrame(0, 1);

	d2d::LibraryPanel* library = static_cast<d2d::LibraryPanel*>(context->library);
	library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());

	context->stage->Refresh();
	context->keysPanel->Refresh();

 	d2d::EditPanel* panel = static_cast<d2d::EditPanel*>(context->stage);
 	panel->getCanvas()->resetViewport();
}

void CommonCMPT::onLoadFromList(wxCommandEvent& event)
{
	Context* context = Context::Instance();

	std::vector<d2d::ISymbol*> symbols;
	Context::Instance()->imagePage->getList()->
		traverse(d2d::FetchAllVisitor<d2d::ISymbol>(symbols));

	if (!symbols.empty())
		context->layers.clear();
	else
		return;

	Layer* layer = new Layer;
	for (size_t i = 0, n = symbols.size(); i < n; ++i)
	{
		KeyFrame* frame = new KeyFrame(i+1);
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbols[i]);
		frame->insertWithClone(sprite);
		layer->insertKeyFrame(frame);
	}
	context->layers.insertLayer(layer);

	context->stage->Refresh();
	context->keysPanel->Refresh();
}

void CommonCMPT::onFillingFrames(wxCommandEvent& event)
{
	Context* context = Context::Instance();

	int tot = m_filling->GetValue();
	for (size_t i = 0, n = context->layers.size(); i < n; ++i)
	{
		Layer* layer = context->layers.getLayer(i);

		const std::map<int, KeyFrame*>& frames = layer->getAllFrames();
		std::vector<KeyFrame*> fixed;
		fixed.reserve(frames.size());
		int dis = tot / frames.size();

		std::map<int, KeyFrame*>::const_iterator itr = frames.begin();
		for (size_t i = 0; itr != frames.end(); ++itr, ++i)
		{
			itr->second->setTime(1+dis*i);
			fixed.push_back(itr->second);
		}

		const_cast<std::map<int, KeyFrame*>&>(frames).clear();
		for (size_t i = 0, n = fixed.size(); i < n; ++i)
			layer->insertKeyFrame(fixed[i]);
	}

	context->stage->Refresh();
	context->keysPanel->Refresh();
}

void CommonCMPT::onChangeAnim(wxCommandEvent& event)
{
	Context::Instance()->resource.choice = event.GetInt();
	FileIO::reload();
}

void CommonCMPT::onAddCross(wxCommandEvent& event)
{
	static_cast<ArrangeSpriteOP*>(m_editOP)->addCross();

}

void CommonCMPT::onDelCross(wxCommandEvent& event)
{
	static_cast<ArrangeSpriteOP*>(m_editOP)->delCross();
}

void CommonCMPT::clear()
{
	Context* context = Context::Instance();
	static_cast<d2d::LibraryPanel*>(context->library)->clear();
	static_cast<StagePanel*>(context->stage)->clear();
	context->layers.clear();
}

}