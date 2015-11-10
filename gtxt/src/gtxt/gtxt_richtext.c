#include "gtxt_richtext.h"
#include "gtxt_label.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define MAX_LAYER	16
#define MAX_FONT	16

struct richtext_state {
	uint32_t color[MAX_LAYER];
	int color_layer;

	int size[MAX_LAYER];
	int size_layer;

	int font[MAX_LAYER];
	int font_layer;

	bool edge[MAX_LAYER];
	int edge_layer;

	struct gtxt_richtext_style style;
};

static char FONTS[MAX_FONT][128];
static int FONT_SIZE = 0;

static void* (*EXT_SYM_CREATE)(const char* str);
static void (*EXT_SYM_RELEASE)(void* ext_sym);
static void (*EXT_SYM_SIZE)(void* ext_sym, int* width, int* height);
static void (*EXT_SYM_RENDER)(void* ext_sym, float x, float y, void* ud);

struct color_map {
	char* name;
	uint32_t color;
};

static const struct color_map COLOR[] = {
	{ "red",		0xff0000ff },
	{ "yellow",		0xffff00ff },
	{ "green",		0x00ff00ff }
};

static inline uint32_t
_parser_color(const char* token) {
	uint32_t col = 0;
	if (token[0] == '#') {
		col = strtoul(&token[1], (char**)NULL, 16);
	} else {
		int num = sizeof(COLOR) / sizeof(struct color_map);
		for (int i = 0; i < num; ++i) {
			if (strcmp(&token[0], COLOR[i].name) == 0) {
				col = COLOR[i].color;
				break;
			}
		}
	}
	return col;
}

void 
gtxt_richtext_add_font(const char* name) {
	strcpy(&FONTS[FONT_SIZE][0], name);
	FONTS[FONT_SIZE][strlen(name) + 1] = 0;
	++FONT_SIZE;
}

void 
gtxt_richtext_ext_sym_cb_init(void* (*create)(const char* str),
							  void (*release)(void* ext_sym),
							  void (*size)(void* ext_sym, int* width, int* height), 
							  void (*render)(void* ext_sym, float x, float y, void* ud)) {
	EXT_SYM_CREATE = create;
	EXT_SYM_RELEASE = release;
	EXT_SYM_SIZE = size;
	EXT_SYM_RENDER = render;
}

void 
gtxt_ext_sym_get_size(void* ext_sym, int* width, int* height) {
	EXT_SYM_SIZE(ext_sym, width, height);
}

void 
gtxt_ext_sym_render(void* ext_sym, float x, float y, void* ud) {
	EXT_SYM_RENDER(ext_sym, x, y, ud);
}

static inline int
_parser_font(const char* token) {
	for (int i = 0; i < MAX_FONT; ++i) {
		if (strcmp(FONTS[i], token) == 0) {
			return i;
		}
	}
	return 0;
}

#define STATE_PUSH(buf, layer, val, ret) { \
	if ((layer) < MAX_LAYER) { \
	(buf)[(layer)++] = (val); \
	(ret) = (val); \
	} else { \
	++(layer); \
	} \
}

#define STATE_POP(buf, layer, ret) { \
	--(layer); \
	assert((layer) >= 0); \
	if ((layer) <= MAX_LAYER) { \
	(ret) = (buf)[(layer) - 1]; \
	} else { \
	(ret) = (buf)[MAX_LAYER - 1]; \
	} \
}

static inline void
_parser_token(const char* token, struct richtext_state* rs) {
	// color
	if (strncmp(token, "color", strlen("color")) == 0) {
		uint32_t col = _parser_color(&token[strlen("color")+1]);
		STATE_PUSH(rs->color, rs->color_layer, col, rs->style.color)
	} else if (strncmp(token, "/color", strlen("/color")) == 0) {
		STATE_POP(rs->color, rs->color_layer, rs->style.color);
	}
	// font
	else if (strncmp(token, "font", strlen("font")) == 0) {
		int font = _parser_font(&token[strlen("font")+1]);
		STATE_PUSH(rs->font, rs->font_layer, font, rs->style.font)
	} else if (strncmp(token, "/font", strlen("/font")) == 0) {
		STATE_POP(rs->font, rs->font_layer, rs->style.font);		
	}	
	// size
	else if (strncmp(token, "size", strlen("size")) == 0) {
		int size = strtol(&token[strlen("size")+1], (char**)NULL, 10);
		STATE_PUSH(rs->size, rs->size_layer, size, rs->style.size)
	} else if (strncmp(token, "/size", strlen("/size")) == 0) {
		STATE_POP(rs->size, rs->size_layer, rs->style.size);				
	}
	// file
	else if (strncmp(token, "file", strlen("file")) == 0) {
		assert(!rs->style.ext_sym_ud);
		rs->style.ext_sym_ud = EXT_SYM_CREATE(&token[strlen("file")+1]);
	} else if (strncmp(token, "/file", strlen("/file")) == 0) {
		EXT_SYM_RELEASE(rs->style.ext_sym_ud);
		rs->style.ext_sym_ud = NULL;
	}
}

static inline void
_init_state(struct richtext_state* rs, struct gtxt_label_style* style) {
	rs->color[0] = style->color;
	rs->color_layer = 1;
	rs->size[0] = style->font_size;
	rs->size_layer = 1;
	rs->font[0] = style->font;
	rs->font_layer = 1;
	rs->edge[0] = style->edge;
	rs->edge_layer = 1;

	rs->style.color	= style->color;
	rs->style.size	= style->font_size;
	rs->style.font	= style->font;
	rs->style.edge	= style->edge;
	rs->style.ext_sym_ud = NULL;
}

void 
gtxt_richtext_parser(const char* str, struct gtxt_label_style* style, 
					 int (*cb)(const char* str, struct gtxt_richtext_style* style, void* ud), void* ud) {
	struct richtext_state rs;
	_init_state(&rs, style);

	int len = strlen(str);
	for (int i = 0; i < len; ) {
		if (str[i] == '<') {
			int j = i;
			while (str[j] != '>' && j < len) {
				++j;
			}
			assert(str[j] == '>');
			char token[j - i];
			strncpy(token, &str[i + 1], j - i - 1);
			token[j - i - 1] = 0;
			_parser_token(token, &rs);
			i = j + 1;
		} else {
			int n = cb(&str[i], &rs.style, ud);
			i += n;
		}
	}
}