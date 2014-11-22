#include "PasteSymbolPhysicsOP.h"

#include "dataset/SpriteFactory.h"
#include "dataset/IBody.h"
#include "view/LibraryPanel.h"
#include "view/PhysicsPanelImpl.h"
#include "view/MultiSpritesImpl.h"
 
namespace d2d
{

PasteSymbolPhysicsOP::PasteSymbolPhysicsOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
										   LibraryPanel* libraryPanel, PhysicsPanelImpl* physicsImpl,
										   float* pScale /*= NULL*/)
	: PasteSymbolOP(editPanel, spritesImpl, libraryPanel, pScale)
	, m_pScale(pScale)
	, m_bStatic(false)
{
	m_world = physicsImpl->getWorld();	
}

bool PasteSymbolPhysicsOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	ISymbol* symbol = m_libraryPanel->getSymbol();
	if (symbol) 
	{
		m_pos = m_editPanel->transPosScreenToProject(x, y);

		ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		sprite->translate(m_pos);

		sprite->loadBodyFromFile();
		if (m_pScale) 
			sprite->setScale(*m_pScale, *m_pScale);
		if (sprite->getBody())
			sprite->getBody()->getBody()->SetType(m_bStatic ? b2_staticBody : b2_dynamicBody);
		m_panelImpl->insertSprite(sprite);
		sprite->Release();
	}

	return false;
}

} // d2d