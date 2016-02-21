#include "PasteSymbolPhysicsOP.h"
#include "PhysicsPanelImpl.h"
#include "IBody.h"
#include "BodyManager.h"
#include "Symbol.h"
#include "LibraryPanel.h"
#include "EditPanelImpl.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "sprite_msg.h"

namespace ee
{

PasteSymbolPhysicsOP::PasteSymbolPhysicsOP(wxWindow* stage_wnd, EditPanelImpl* stage, 
										   LibraryPanel* library, PhysicsPanelImpl* physicsImpl, 
										   float* pScale /*= NULL*/)
	: PasteSymbolOP(stage_wnd, stage, library, pScale)
	, m_pScale(pScale)
	, m_bStatic(false)
{
	m_world = physicsImpl->getWorld();	
}

bool PasteSymbolPhysicsOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	Symbol* symbol = m_library->GetSymbol();
	if (symbol) 
	{
		m_pos = m_stage->TransPosScrToProj(x, y);

		Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
		sprite->Translate(m_pos);
		if (m_pScale) {
			sprite->SetScale(Vector(*m_pScale, *m_pScale));
		}

		IBody* body = BodyManager::Instance()->LoadBody(sprite);
		if (body) {
			body->GetBody()->SetType(m_bStatic ? b2_staticBody : b2_dynamicBody);
		}
		InsertSpriteSJ::Instance()->Insert(sprite);

		sprite->Release();
	}

	return false;
}

}
