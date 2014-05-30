#ifndef _SG_TOOLS_H_
#define _SG_TOOLS_H_

namespace sg
{

static bool IsSymbolWall(d2d::ISprite* sprite)
{
	SymbolInfo* info = static_cast<SymbolInfo*>(sprite->getSymbol().getUserData());
	if (info == NULL) {
		return false;
	}

	return info->iswall;
}

}

#endif // _SG_TOOLS_H_