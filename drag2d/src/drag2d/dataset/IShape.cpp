#include "IShape.h"

#include <wx/wx.h>

namespace d2d
{

IShape::IShape() 
{ 
	static int index = 0;
	name = "shape" + wxString::FromDouble(index++);
}

} // d2d