#ifndef _ESHADER_STAGE_CANVAS3D_H_
#define _ESHADER_STAGE_CANVAS3D_H_

#include <easysketch.h>

namespace eshader
{

class StageCanvas3D : public libsketch::StageCanvas
{
public:
	StageCanvas3D(d2d::EditPanel* edit_panel, d2d::MultiSpritesImpl* stage, 
		d2d::LibraryPanel* library);

}; // StageCanvas3D

}

#endif // _ESHADER_STAGE_CANVAS3D_H_