#ifndef _SG_TOOLS_H_
#define _SG_TOOLS_H_

namespace sg
{

static bool IsSymbolWall(const ee::Symbol& symbol)
{
	SymbolExt* info = static_cast<SymbolExt*>(symbol.GetUserData());
	if (info == NULL) {
		return false;
	}

	return info->wall_type != -1;
}

static bool IsSymbolWall(const ee::Sprite& sprite)
{
	return IsSymbolWall(sprite.GetSymbol());
}

}

#endif // _SG_TOOLS_H_