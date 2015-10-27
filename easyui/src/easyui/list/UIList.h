#ifndef _EASYUI_LIST_UI_LIST_H_
#define _EASYUI_LIST_UI_LIST_H_

#include <drag2d.h>

namespace eui
{
namespace list
{

class UIList
{
public:	
	d2d::Rect clipbox;

	d2d::SpritesContainer bgs;
	d2d::SpritesContainer items;

	bool vertical;

}; // UIList

}
}

#endif // _EASYUI_LIST_UI_LIST_H_