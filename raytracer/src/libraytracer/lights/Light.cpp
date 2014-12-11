#include "Light.h"

namespace rt
{

Light::Light()
	: m_shadows(true)
{
}

bool Light::CastsShadows() const
{
	return m_shadows;
}

}