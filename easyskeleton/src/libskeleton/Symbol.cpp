#include "Symbol.h"
#include "Sprite.h"

#include <sprite2/Skeleton.h>
#include <gum/SkeletonSymLoader.h>

namespace libskeleton
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

void Symbol::LoadResources()
{
	gum::SkeletonSymLoader loader(this);
	loader.LoadJson(m_filepath);
}

}