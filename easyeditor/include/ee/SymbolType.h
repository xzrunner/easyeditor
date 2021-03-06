#ifndef _EASYEDITOR_SYMBOL_TYPE_H_
#define _EASYEDITOR_SYMBOL_TYPE_H_

namespace ee
{

enum SymType
{
	SYM_ANIS = 1000,
	SYM_FONTBLANK,
	SYM_FREETYPE,
	SYM_SCRIPTS,
	SYM_P3DINV,
	SYM_EJOY2D,
	SYM_TERRAIN2D,
	SYM_SHADOW,
	SYM_UI,
	SYM_PSD,
	SYM_UIWND,
};

static const char* SYM_GROUP_TAG = "group";
static const char* SYM_SHAPE_TAG = "shape";
static const char* SYM_TEXT_TAG = "text";

}

#endif // _EASYEDITOR_SYMBOL_TYPE_H_