#ifndef _DRAG2D_STYLE_CONFIG_H_
#define _DRAG2D_STYLE_CONFIG_H_

#include "common/color_config.h"

namespace d2d
{

static const ShapeStyle LIGHT_GREY_THIN_LINE(false, 1, LIGHT_GREY, LS_DEFAULT);
static const ShapeStyle LIGHT_GREY_LINE(false, 2, LIGHT_GREY, LS_DEFAULT);

static const ShapeStyle LIGHT_RED_THIN_LINE(false, 1, MID_RED, LS_DEFAULT);
static const ShapeStyle LIGHT_RED_LINE(false, 2, MID_RED, LS_DEFAULT);

static const ShapeStyle BLACK_RECT_FACE(true, BLACK);

static const ShapeStyle SELECT_ALL(true, 2, LIGHT_GREEN, LS_DEFAULT);
static const ShapeStyle SELECT_PART(true, 2, LIGHT_BLUE, LS_DEFAULT);
static const ShapeStyle SELECT_BOUND(false, 2, SELECT_RED, LS_DEFAULT);

static const ShapeStyle LIGHT_GREEN_FACE(true, 2, LIGHT_GREEN, LS_DEFAULT);
static const ShapeStyle LIGHT_RED_FACE(true, 2, LIGHT_RED, LS_DEFAULT);

}

#endif // _DRAG2D_STYLE_CONFIG_H_