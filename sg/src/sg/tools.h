#ifndef _SG_TOOLS_H_
#define _SG_TOOLS_H_

#include <ee/Symbol.h>
#include <ee/Sprite.h>

namespace sg
{

static bool IsSymbolWall(const ee::Symbol& sym)
{
	SymbolExt* info = static_cast<SymbolExt*>(sym.GetUserData());
	if (info == NULL) {
		return false;
	}

	return info->wall_type != -1;
}

static bool IsSymbolWall(const ee::Sprite& spr)
{
	return IsSymbolWall(spr.GetSymbol());
}

}

#endif // _SG_TOOLS_H_