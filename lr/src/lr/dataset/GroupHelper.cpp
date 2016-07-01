#include "GroupHelper.h"
#include "view/typedef.h"

#include <easycomplex.h>

namespace lr
{

ee::Sprite* GroupHelper::Group(const std::vector<ee::Sprite*>& sprites,
							   const std::string& name)
{
	ecomplex::Sprite* spr = ecomplex::GroupHelper::Group(sprites);
	ecomplex::Symbol& sym = const_cast<ecomplex::Symbol&>(spr->GetSymbol());
	sym.SetFilepath(GROUP_TAG);
	sym.name = name;
	sym.SetName(name);
	spr->SetName(name);
	return spr;
}

void GroupHelper::BreakUp(ee::Sprite* group, std::vector<ee::Sprite*>& sprites)
{
	ecomplex::GroupHelper::BreakUp(group, sprites);
}

}