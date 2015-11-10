#include "gtxt_richtext.h"
#include "gtxt_label.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define MAX_LAYER 16

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

struct color_map {
	char* name;
	uint32_t color;
};

static const struct color_map COLOR[] = {
	{ "red",		0xff0000ff },
	{ "yellow",		0xffff00ff },
	{ "green",		0x00ff00ff }
};

static inline void
_parser_token(const char* token, struct richtext_state* rs) {
	// color
	if (strncmp(token, "color", strlen("color"))) {
		uint32_t col = 0;
		if (token[6] == '#') {
			col = strtol(&token[7], (char**)NULL, 16);
		} else {
			int num = sizeof(COLOR) / sizeof(struct color_map);
			for (int i = 0; i < num; ++i) {
				if (strcmp(&token[6], COLOR[i].name) == 0) {
					col = COLOR[i].color;
					break;
				}
			}
		}
	} else if (strncmp(token, "/color", strlen("/color"))) {
		--rs->color_layer;
		assert(rs->color_layer >= 0);
	}
	// font
	else if (strncmp(token, "font", strlen("font"))) {

	} else if (strncmp(token, "/font", strlen("/font"))) {

	}	
	// size
	else if (strncmp(token, "size", strlen("size"))) {
		
	} else if (strncmp(token, "/size", strlen("/size"))) {

	}
	// file
	else if (strncmp(token, "file", strlen("file"))) {
		// <file=img.png>
		// <file=pkg,spr>
	} else if (strncmp(token, "/file", strlen("/file"))) {
		// 
	}
}

static inline void
_init_state(struct richtext_state* rs, struct gtxt_label_style* style) {
	rs->color[0] = style->color;
	rs->color_layer++;
	rs->size[0] = style->font_size;
	rs->size_layer++;
	rs->font[0] = style->font;
	rs->font_layer++;
	rs->edge[0] = style->edge;
	rs->edge_layer++;

	rs->style.color	= style->color;
	rs->style.size	= style->font_size;
	rs->style.font	= style->font;
	rs->style.edge	= style->edge;
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
			strncpy(token, &str[i + 1], j - i - i);
			token[j - i - 1] = 0;
			_parser_token(token, &rs);
			i = j + 1;
		} else {
			int n = cb(&str[i], &rs.style, ud);
			i += n;
		}
	}
}