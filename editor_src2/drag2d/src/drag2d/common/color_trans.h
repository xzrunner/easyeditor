#ifndef	D2D_COLOR_TRANS_H
#define D2D_COLOR_TRANS_H

#include "Color.h"

namespace d2d
{
	Colorf transColor(const std::string& str);
	float transColor(char high, char low);
	int transHex(char c);

	std::string transColor(const Colorf& col);
	std::string transColor(float col);
	char transHex(int v);
}

#endif // D2D_COLOR_TRANS_H