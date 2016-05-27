#ifndef _SPRITE2_TEXTBOX_H_
#define _SPRITE2_TEXTBOX_H_

#include "Color.h"

namespace s2
{

class Textbox
{
public:
	enum HoriAlign
	{
		HA_LEFT = 0,
		HA_RIGHT,
		HA_CENTER,
		HA_AUTO
	};

	enum VertAlign
	{
		VA_TOP = 0,
		VA_BOTTOM,
		VA_CENTER,
		VA_AUTO
	};

public:
	int			width;
	int			height;

	int			font_type;
	int			font_size;
	Color		font_color;

	bool		has_edge;
	float		edge_size;
	Color		edge_color;

	HoriAlign	align_hori;
	VertAlign	align_vert;

	float		space_hori;
	float		space_vert;

}; // Textbox

}

#endif // _SPRITE2_TEXTBOX_H_