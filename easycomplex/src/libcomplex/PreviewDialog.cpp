#include "PreviewDialog.h"
#include "PreviewStage.h"
#include "PreviewCanvas.h"
#include "PreviewEditOP.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/Visitor.h>

#include <easyanim.h>

namespace ecomplex
{

class InitVisitor : public ee::Visitor<ee::Sprite>
{
public:
	virtual void Visit(ee::Sprite* spr, bool& next) {
		if (eanim::Sprite* anim = dynamic_cast<eanim::Sprite*>(spr)) {
			dynamic_cast<eanim::Symbol*>(anim->GetSymbol())->SetLoop(false);
		}
	}
}; // InitVisitor

PreviewDialog::PreviewDialog(wxWindow* parent, wxGLContext* glctx,
							 const std::vector<const ee::Sprite*>& sprites)
	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
	, m_sprites(sprites)
	, m_control(0.033f)
{
	InitLayout(glctx);

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	data.particle3d_loop = false;

	InitVisitor init;
	bool next;
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		ee::Sprite* spr = const_cast<ee::Sprite*>(m_sprites[i]);
		init.Visit(spr, next);
		dynamic_cast<ee::Symbol*>(spr->GetSymbol())->Traverse(init);
	}
}

PreviewDialog::~PreviewDialog()
{
	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	data.particle3d_loop = true;

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		if (const eanim::Sprite* anim = dynamic_cast<const eanim::Sprite*>(m_sprites[i])) {
			const_cast<eanim::Symbol*>(dynamic_cast<const eanim::Symbol*>(anim->GetSymbol()))->SetLoop(true);
		}
	}
}

void PreviewDialog::InitLayout(wxGLContext* glctx)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	BuildEditPanel(sizer, glctx);
	SetSizer(sizer);
}

void PreviewDialog::BuildEditPanel(wxSizer* sizer, wxGLContext* glctx)
{
	PreviewStage* stage = new PreviewStage(this, this, m_control);
	stage->SetEditOP(new PreviewEditOP(stage, stage->GetStageImpl(), m_sprites));
	stage->SetCanvas(new PreviewCanvas(stage, stage->GetStageImpl(), m_sprites, glctx));
	sizer->Add(stage, 1, wxEXPAND);
}

}