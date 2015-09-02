#ifndef _EPBIN_EPE_TYPE_H_
#define _EPBIN_EPE_TYPE_H_

#include <stdint.h>

namespace epbin
{
namespace epe
{

static const uint8_t TAG_ID			= 1;
static const uint8_t TAG_COLOR		= 2;
static const uint8_t TAG_ADDITIVE	= 4;
static const uint8_t TAG_MATRIX		= 8;
static const uint8_t TAG_TOUCH		= 16;
static const uint8_t TAG_MATRIXREF	= 32;
static const uint8_t TAG_COLMAP		= 64;

}
}

#endif // _EPBIN_EPE_TYPE_H_