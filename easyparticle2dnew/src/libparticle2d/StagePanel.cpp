#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "LibraryPanel.h"
#include "ParticleSystem.h"
#include "Symbol.h"

namespace eparticle2d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
{
	m_canvas = new StageCanvas(this);

	d2d::ArrangeSpriteConfig cfg;
	cfg.is_auto_align_open = false;
	cfg.is_deform_open = false;
	cfg.is_offset_open = false;
	cfg.is_rotate_open = false;
	m_editOP = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, NULL, NULL, cfg);
}

void StagePanel::SetSelectedSymbol(Symbol* symbol)
{
	m_data.SetSelected(symbol);
}

void StagePanel::SetToolbar(ToolbarPanel* toolbar)
{
	m_data.SetToolbar(toolbar);
}

void StagePanel::UpdatePS(float dt)
{
	ParticleSystem* ps = m_data.GetPS();
	if (ps) {
		ps->Update(dt);
	}

	std::set<ParticleSystem*> all_ps;
	std::vector<d2d::ISprite*> sprites;
	traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		const Symbol& symbol = static_cast<const Symbol&>(sprites[i]->getSymbol());
		all_ps.insert(const_cast<Symbol&>(symbol).GetPS());
	}

	std::set<ParticleSystem*>::iterator itr = all_ps.begin();
	for ( ; itr != all_ps.end(); ++itr) {
		(*itr)->Update(dt);
	}
}

}