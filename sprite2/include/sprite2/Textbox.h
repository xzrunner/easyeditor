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
		HA_LEFT		= 0,
		HA_RIGHT	= 1,
		HA_CENTER	= 2,
		HA_AUTO		= 3
	};

	enum VertAlign
	{
		VA_TOP		= 0,
		VA_BOTTOM	= 1,
		VA_CENTER	= 2,
		VA_AUTO		= 3
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

	bool		overflow;

	bool		richtext;

public:
	Textbox() 
		: width(100)
		, height(20)
		, font_type(0)
		, font_size(16)
		, font_color(0xffffffff)
		, has_edge(false)
		, edge_size(1)
		, edge_color(0)
		, align_hori(HA_LEFT)
		, align_vert(VA_TOP)
		, space_hori(1)
		, space_vert(1)
		, overflow(true)
		, richtext(true)
	{
	}

}; // Textbox

}

#endif // _SPRITE2_TEXTBOX_H_