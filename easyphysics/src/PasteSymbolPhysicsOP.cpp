#include "PasteSymbolPhysicsOP.h"
#include "PhysicsPanelImpl.h"
#include "IBody.h"
#include "BodyManager.h"

namespace ephysics
{

PasteSymbolPhysicsOP::PasteSymbolPhysicsOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
										   d2d::LibraryPanel* libraryPanel, PhysicsPanelImpl* physicsImpl,
										   float* pScale /*= NULL*/)
	: d2d::PasteSymbolOP(editPanel, spritesImpl, libraryPanel, pScale)
	, m_pScale(pScale)
	, m_bStatic(false)
{
	m_world = physicsImpl->getWorld();	
}

bool PasteSymbolPhysicsOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	d2d::ISymbol* symbol = m_libraryPanel->getSymbol();
	if (symbol) 
	{
		m_pos = m_editPanel->transPosScreenToProject(x, y);

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->translate(m_pos);
		if (m_pScale) {
			sprite->setScale(*m_pScale, *m_pScale);
		}

		IBody* body = BodyManager::Instance()->LoadBody(sprite);
		if (body) {
			body->getBody()->SetType(m_bStatic ? b2_staticBody : b2_dynamicBody);
		}
		m_panelImpl->insertSprite(sprite);

		sprite->Release();
	}

	return false;
}

}
