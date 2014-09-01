#ifndef D2D_TYPES_H
#define D2D_TYPES_H

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;

namespace d2d
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

	enum HoriAlignType
	{
		HAT_LEFT = 0,
		HAT_RIGHT = 1,
		HAT_CENTER = 2,
		HAT_AUTO = 3
	};

	enum VertAlignType
	{
		VAT_TOP = 0,
		VAT_BOTTOM = 1,
		VAT_CENTER = 2,
		VAT_AUTO = 3
	};
}

#endif // D2D_TYPES_H