#include "DummySymbol.h"

namespace ee
{

DummySymbol::DummySymbol(const std::string& filepath)
{
	m_filepath = filepath;
}

DummySymbol::DummySymbol(const std::string& filepath, int width, int height)
	: m_bounding(static_cast<float>(width), static_cast<float>(height))
{ 
	m_filepath = filepath;
}

sm::rect DummySymbol::GetBounding(const s2::Sprite* spr, const s2::Actor* actor) const
{
	return m_bounding;
}

}