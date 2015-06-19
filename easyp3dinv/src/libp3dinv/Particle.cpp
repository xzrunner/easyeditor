#include "Particle.h"

namespace ep3dinv
{

Particle::Particle()
	: m_symbol(NULL)
{
}

Particle::~Particle()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

void Particle::LoadFromFile(const Json::Value& val)
{
	
}

}