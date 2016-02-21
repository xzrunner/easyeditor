#ifndef _EASYEDITOR_SCRIPT_TYPE_H_
#define _EASYEDITOR_SCRIPT_TYPE_H_

#include <stdint.h>

namespace ee
{

enum Type
{
	INVALID_ID = 0,
	// Cross-module types.
	OBJECT_ID,
	DATA_ID,
	MODULE_ID,

	// Sprite
	SPRITE_DATA_ID,
	SPRITE_SELECTION_DATA_ID,

	TYPE_MAX_ENUM
};

const uint32_t INVALID_T = 1 << INVALID_ID;

const uint32_t OBJECT_T = 1 << OBJECT_ID;
const uint32_t DATA_T = (1 << DATA_ID) | OBJECT_T;
const uint32_t MODULE_T = (1 << MODULE_ID) | OBJECT_T;

// Sprite
const uint32_t SPRITE_DATA_T = (1 << SPRITE_DATA_ID) | OBJECT_T;
const uint32_t SPRITE_SELECTION_DATA_T = (1 << SPRITE_SELECTION_DATA_ID) | OBJECT_T;

}

#endif // _EASYEDITOR_SCRIPT_TYPE_H_
