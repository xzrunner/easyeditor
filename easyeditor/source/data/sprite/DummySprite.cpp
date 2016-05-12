#include "DummySprite.h"

#include <sprite2/DummySprite.h>

namespace ee
{

DummySprite::DummySprite()
	: m_symbol(NULL)
{
	m_core = new s2::DummySprite;
}

DummySprite::DummySprite(Symbol* symbol) 
	: m_symbol(symbol) 
{
	m_core = new s2::DummySprite;
	m_symbol->Retain();
}

DummySprite::~DummySprite() 
{
	delete m_core; m_core = NULL;

	if (m_symbol) {
		m_symbol->Release();
	}
}

}