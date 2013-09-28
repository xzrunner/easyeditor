#include "ToolbarPanel.h"

#include "frame/Context.h"
#include "frame/FileIO.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"

namespace eanim
{

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: d2d::ToolbarPanel(parent, static_cast<d2d::EditPanel*>(Context::Instance()->stage))
	, m_animChoice(NULL)
{
	SetSizer(initLayout());
}

void ToolbarPanel::addAnimChoice(const std::vector<std::string>& choices)
{
	wxSizer* sizer = GetSizer();

	if (m_animChoice) 
		delete m_animChoice;

	wxArrayString array;
	for (size_t i = 0, n = choices.size(); i < n; ++i)
		array.push_back(choices[i]);

	m_animChoice = new wxRadioBox(this, wxID_ANY, wxT("anims"), wxDefaultPosition, wxDefaultSize, array, 1, wxRA_SPECIFY_COLS);
	Connect(m_animChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(ToolbarPanel::onChangeAnim));
	sizer->Add(m_animChoice);

	Layout();
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	// load all image
	{
		wxButton* btnLoad = new wxButton(this, wxID_ANY, wxT("Load All"));
		Connect(btnLoad->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(ToolbarPanel::onLoadAllFrameImages));
		sizer->Add(btnLoad);
	}
	sizer->AddSpacer(10);
	// fill frames
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Filling"));
		wxBoxSizer* fillingSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("tot frames: ")));

			m_filling = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
				wxSP_ARROW_KEYS, 10, 1000, 0);
			sizer->Add(m_filling);

			fillingSizer->Add(sizer);
		}
		{
			wxButton* btnFill = new wxButton(this, wxID_ANY, wxT("Filling"));
			Connect(btnFill->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
				wxCommandEventHandler(ToolbarPanel::onFillingFrames));
			fillingSizer->Add(btnFill);
		}
		sizer->Add(fillingSizer);
	}
	sizer->AddSpacer(10);
	return sizer;
}

void ToolbarPanel::onLoadAllFrameImages(wxCommandEvent& event)
{
	Context* context = Context::Instance();

	std::vector<d2d::ISymbol*> symbols;
	Context::Instance()->imagePage->getList()->
		traverse(d2d::FetchAllVisitor<d2d::ISymbol>(symbols));

	if (!symbols.empty())
		context->layers.clear();

	Layer* layer = new Layer;
	for (size_t i = 0, n = symbols.size(); i < n; ++i)
	{
		KeyFrame* frame = new KeyFrame(i+1);
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbols[i]);
		frame->getAllSprites().push_back(sprite);
		layer->insertKeyFrame(frame);
	}
	context->layers.insertLayer(layer);

	context->stage->Refresh();
	context->keysPanel->Refresh();
}

void ToolbarPanel::onFillingFrames(wxCommandEvent& event)
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

void ToolbarPanel::onChangeAnim(wxCommandEvent& event)
{
	Context::Instance()->resource.choice = event.GetInt();
	FileIO::reload();
}
} // eanim