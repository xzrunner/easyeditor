#ifndef _LIBCOCO_PACK_LABEL_H_
#define _LIBCOCO_PACK_LABEL_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace libcoco
{

class PackLabel : public IPackNode
{
public:
	PackLabel(const d2d::FontSprite* font);

private:
	void Load(const d2d::FontSprite* font);
	
private:
	std::string m_name;
	std::string m_font;

	d2d::HoriAlignType m_align_hori;
	d2d::VertAlignType m_align_vert;

	d2d::Colorf m_color;	// PT_ARGB

	int m_size;
	int m_width, m_height;

	bool m_edge;

}; // PackLabel

}

#endif // _LIBCOCO_PACK_LABEL_H_