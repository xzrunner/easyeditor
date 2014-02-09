#include "CommonCMPT.h"
#include "ArrangeSpriteOP.h"

#include "view/StagePanel.h"
#include "frame/Context.h"
#include "frame/FileIO.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"

namespace eanim
{
	CommonCMPT::CommonCMPT(wxWindow* parent, const wxString& name, 
		StagePanel* stage)
		: d2d::AbstractEditCMPT(parent, name, stage)
	{
		m_editOP = new ArrangeSpriteOP(stage);
	}

	wxSizer* CommonCMPT::initLayout()
	{
		return initEditPanel();
	}

	wxSizer* CommonCMPT::initEditPanel()
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		sizer->AddSpacer(20);
		// load all image
		{
			wxButton* btnLoad = new wxButton(this, wxID_ANY, wxT("Load All"));
			Connect(btnLoad->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
				wxCommandEventHandler(CommonCMPT::onLoadAllFrameImages));
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
					wxCommandEventHandler(CommonCMPT::onFillingFrames));
				fillingSizer->Add(btnFill);
			}
			sizer->Add(fillingSizer);
		}
		sizer->AddSpacer(10);
		return sizer;
	}

	void CommonCMPT::onLoadAllFrameImages(wxCommandEvent& event)
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
			frame->insert(sprite);
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

}