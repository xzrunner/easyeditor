#ifndef _LIBCOCO_PACK_LABEL_H_
#define _LIBCOCO_PACK_LABEL_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace libcoco
{

class PackLabel : public IPackNode
{
public:
	std::string name;
	std::string font;

	d2d::HoriAlignType align_hori;
	d2d::VertAlignType align_vert;

	d2d::Colorf color;	// PT_ARGB

	int size;
	int width, height;

	bool has_edge;

}; // PackLabel

}

#endif // _LIBCOCO_PACK_LABEL_H_