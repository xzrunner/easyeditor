#include "StageCanvas3D.h"

namespace eshader
{

StageCanvas3D::StageCanvas3D(d2d::EditPanel* edit_panel, d2d::MultiSpritesImpl* stage, 
						 d2d::LibraryPanel* library)
	: libsketch::StageCanvas(edit_panel, stage, library)
{
}

}