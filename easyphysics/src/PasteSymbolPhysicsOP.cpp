#include "PasteSymbolPhysicsOP.h"
#include "PhysicsPanelImpl.h"
#include "IBody.h"
#include "BodyManager.h"

namespace ephysics
{

PasteSymbolPhysicsOP::PasteSymbolPhysicsOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
										   ee::LibraryPanel* libraryPanel, PhysicsPanelImpl* physicsImpl, 
										   float* pScale /*= NULL*/)
	: ee::PasteSymbolOP(stage_wnd, stage, libraryPanel, pScale)
	, m_pScale(pScale)
	, m_bStatic(false)
{
	m_world = physicsImpl->getWorld();	
}

bool PasteSymbolPhysicsOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	ee::Symbol* symbol = m_libraryPanel->GetSymbol();
	if (symbol) 
	{
		m_pos = m_stage->TransPosScrToProj(x, y);

		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		sprite->Translate(m_pos);
		if (m_pScale) {
			sprite->SetScale(ee::Vector(*m_pScale, *m_pScale));
		}

		IBody* body = BodyManager::Instance()->LoadBody(sprite);
		if (body) {
			body->getBody()->SetType(m_bStatic ? b2_staticBody : b2_dynamicBody);
		}
		ee::InsertSpriteSJ::Instance()->Insert(sprite);

		sprite->Release();
	}

	return false;
}

}
