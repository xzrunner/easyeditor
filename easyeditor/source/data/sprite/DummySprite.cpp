#include "DummySprite.h"

#include <sprite2/DummySprite.h>

namespace ee
{

DummySprite::DummySprite()
	: m_symbol(NULL)
{
	m_core = new s2::DummySprite(this);
}

DummySprite::DummySprite(Symbol* symbol) 
	: m_symbol(symbol) 
{
	m_core = new s2::DummySprite(this);
	m_symbol->AddReference();
}

DummySprite::~DummySprite() 
{
	m_core->RemoveReference();

	if (m_symbol) {
		m_symbol->RemoveReference();
	}
}

}