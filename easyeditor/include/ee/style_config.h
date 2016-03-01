#ifndef _EASYEDITOR_STYLE_CONFIG_H_
#define _EASYEDITOR_STYLE_CONFIG_H_

#include "color_config.h"
#include "ShapeStyle.h"

namespace ee
{

static const ShapeStyle LIGHT_GREY_THIN_LINE(false, 1, LIGHT_GREY);
static const ShapeStyle LIGHT_GREY_LINE(false, 2, LIGHT_GREY);

static const ShapeStyle LIGHT_RED_THIN_LINE(false, 1, MID_RED);
static const ShapeStyle LIGHT_RED_LINE(false, 2, MID_RED);

static const ShapeStyle LIGHT_GREEN_THIN_LINE(false, 1, LIGHT_GREEN);
static const ShapeStyle LIGHT_GREEN_LINE(false, 2, LIGHT_GREEN);

static const ShapeStyle BLACK_RECT_FACE(true, 2, BLACK);

static const ShapeStyle SELECT_ALL(true, 2, LIGHT_GREEN);
static const ShapeStyle SELECT_PART(true, 2, LIGHT_BLUE);
static const ShapeStyle SELECT_BOUND(false, 2, SELECT_RED);

static const ShapeStyle LIGHT_GREEN_FACE(true, 2, LIGHT_GREEN);
static const ShapeStyle LIGHT_RED_FACE(true, 2, LIGHT_RED);

}

#endif // _EASYEDITOR_STYLE_CONFIG_H_