#ifndef _EASYEDITOR_COLOR_CONFIG_H_
#define _EASYEDITOR_COLOR_CONFIG_H_

#include "Color.h"

namespace ee
{

static Colorf NULL_COLOR(0, 0, 0, 0);

static Colorf LIGHT_GREY(0.8f, 0.8f, 0.8f, 0.8f);
static Colorf LIGHT_GREY_ALPHA(0.8f, 0.8f, 0.8f, 0.4f);

static Colorf MID_RED(0.8f, 0, 0, 0.8f);
static Colorf LIGHT_RED(0.22f, 0, 0, 0.5f);

static Colorf LIGHT_GREEN(0, 0.22f, 0, 0.5f);

static Colorf LIGHT_BLUE(0, 0, 0.22f, 0.5f);

static Colorf BLACK(0, 0, 0, 1);
static Colorf WHITE(1, 1, 1, 1);
static Colorf RED(1, 0, 0, 1);
static Colorf GREEN(0, 1, 0, 1);
static Colorf BLUE(1, 0, 1, 1);

static Colorf SELECT_RED(0.75f, 0, 0, 1);

}

#endif // _EASYEDITOR_COLOR_CONFIG_H_