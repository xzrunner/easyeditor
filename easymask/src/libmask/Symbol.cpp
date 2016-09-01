#include "Symbol.h"
#include "FileIO.h"

#include <sprite2/S2_Sprite.h>

namespace emask
{

void Symbol::LoadResources()
{
	FileIO::Load(m_filepath.c_str(), this);
}

void Symbol::SetBase(s2::Sprite* base)
{
	cu::RefCountObjAssign(m_base, base);
}

void Symbol::SetMask(s2::Sprite* mask)
{
	cu::RefCountObjAssign(m_mask, mask);
}

}