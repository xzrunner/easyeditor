#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// lzma file

#include "LzmaDec.h"
#include "Alloc.h"
#define LZMA_PROPS_SIZE 5

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

static int
LzmaUncompress(void *dest, size_t  *destLen, const void *src, size_t  *srcLen,
  const void *props, size_t propsSize)
{
  ELzmaStatus status;
  return LzmaDecode(dest, destLen, src, srcLen, props, (unsigned)propsSize, LZMA_FINISH_ANY, &status, &g_Alloc);
}

struct ZFILE {
	size_t size;
	char * ptr;
	char * buffer;
};

struct ZFILE *
zfopen(const char * filename) {
	FILE * f= fopen(filename,"rb");
	fseek(f,0,SEEK_END);
	size_t src_sz = ftell(f) - 9;
	fseek(f,0,SEEK_SET);
	char props[5];
	uint32_t dst_sz;
	int r = fread(props, sizeof(props), 1, f);
	if (r == 0) {
		printf("zfopen: read props failed\n");
		fclose(f);
		return NULL;
	}
	r = fread(&dst_sz, sizeof(dst_sz), 1, f);
	if (r == 0) {
		printf("zfopen: read size failed\n");
		fclose(f);
		return NULL;
	}
	char * tmp = malloc(src_sz);
	r = fread(tmp, src_sz, 1, f);
	if (r == 0) {
		printf("zfopen: read data failed\n");
		fclose(f);
		return NULL;
	}
	struct ZFILE *zf = malloc(sizeof(*zf));
	zf->size = dst_sz;
	zf->ptr = zf->buffer = malloc(dst_sz);
	r = LzmaUncompress(zf->buffer, &zf->size, tmp, &src_sz, props, LZMA_PROPS_SIZE);
	free(tmp);
	if (r!= SZ_OK) {
		free(zf->buffer);
		free(zf);
		return NULL;
	}
	return zf;
}

void
zfclose(struct ZFILE * zf) {
	if (zf) {
		free(zf->buffer);
		free(zf);
	}
}

int
zfread(void *buf, int sz, int count, struct ZFILE *zf) {
	int rd = zf->size - (zf->ptr - zf->buffer);
	if (rd < sz)
		return 0;
	int r = rd/sz;
	if (r > count) {
		r = count;
	}
	if (buf) {
		memcpy(buf, zf->ptr, r * sz);
	}
	zf->ptr += r*sz;
	return r;
}

int
zfeof(struct ZFILE *f) {
	return f->ptr == f->buffer;
}

// lzma end

#define MAX_SPRITE 0x10000
#define MAX_EXPORT 8192

#define SPRITE_PICTURE 0
#define SPRITE_ANIMATION 1
#define SPRITE_TEXT 2

static void
fault(const char * format, ...) {
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	exit(1);
}

struct screen_coord {
	float wscale;
	float hscale;
	int xoffset;
	int yoffset;
	int width;
	int height;
};

struct name {
	int id;
	char * str;
};

struct fragment {
	struct {
		int x;
		int y;
	} dest[4];
	struct {
		uint16_t x;
		uint16_t y;
	} src[4];
	int texid;
	uint8_t unknown[5];
};

struct picture {
	int n;
	struct fragment *f;
};

struct animation_pic {
	int id;
	int attrib;
	char * name;
};

struct blend_table {
	uint32_t mul;
	uint32_t add;
};

struct animation_part {
	int index;
	int *m;
	struct blend_table color;
};

struct animation_frame {
	int index;
	int n;
	char * name;
};

struct animation {
	int frame_n;
	int part_n;
	int pic_n;
	int type;
	char res[5];
	struct animation_pic * pic;
	struct animation_part * part;
	struct animation_frame * frame;
};

struct text {
	uint32_t color;
	char * font;
	int size;
	int width;
	int height;
	int align;
	uint8_t res[16];
};

struct sprite_res {
	int type;
	union {
		struct picture p;
		struct animation a;
		struct text t;
	} u;
};

struct offset_table {
	int m[6];
};

struct sc_decode {
	int magic;
	int texture_base;
	int sprite_base;
	int max_sprite;
	int texture;
	int picture;
	int animation;
	int offset;
	int offset_count;
	struct offset_table *ot;
	int alpha;
	int alpha_count;
	struct blend_table *bt;
};

static int SpriteNumber = 0;
static struct sprite_res * Sprite[MAX_SPRITE];
static int ExportNumber = 0;
static struct name Export[MAX_EXPORT];
static int TextureNumber = 0;
static const char * FileName = NULL;

//   begin sc

static uint32_t
read_word(struct ZFILE *f) {
	uint16_t v=0;
	int r = zfread(&v, sizeof(v), 1, f);
	if (r == 0) {
		fault("Invalid file\n");
	}
	return v;
}

static uint32_t
read_dword(struct ZFILE *f) {
	uint32_t v=0;
	int r = zfread(&v, sizeof(v), 1, f);
	if (r == 0) {
		fault("Invalid file\n");
	}
	return v;
}

static uint32_t
read_byte(struct ZFILE *f) {
	uint8_t v = 0;
	int r = zfread(&v, sizeof(v), 1, f);
	if (r == 0) {
		fault("Invalid file\n");
	}
	return (uint32_t)v;
}

static void
read_bytes(struct ZFILE *f, char *buffer, int sz) {
	int r = zfread(buffer, sz, 1, f);
	if (r == 0) {
		fault("Invalid file\n");
	}
}

static void
write_ppm8(uint8_t *data, int n, int width, int height) {
	char tmp[1024];
	printf("Writing 8888 %s.%d.ppm\n",FileName,n);
	sprintf(tmp,"%s.%d.ppm",FileName, n);
	FILE *f = fopen(tmp,"wb");
	if (f==NULL) {
		fault("Can't write to %s\n",tmp);
	}
	fprintf(f,"P6\n%d %d\n255\n",width,height);
	int i;
	for (i=0;i<width*height*4;i+=4) {
		fputc(data[i],f);
		fputc(data[i+1],f);
		fputc(data[i+2],f);
	}
	fclose(f);
	sprintf(tmp,"%s.%d.alpha.pgm",FileName,n);
	f = fopen(tmp,"wb");
	if (f==NULL) {
		fault("Can't write to %s\n",tmp);
	}
	fprintf(f,"P5\n%d %d\n255\n",width,height);
	for (i=0;i<width*height*4;i+=4) {
		fputc(data[i+3],f);
	}
	fclose(f);
}

static void
write_ppm4(uint8_t *data, int n, int width, int height) {
	char tmp[1024];
	printf("Writing 4444 %s.%d.ppm\n",FileName,n);
	sprintf(tmp,"%s.%d.ppm",FileName, n);
	FILE *f = fopen(tmp,"wb");
	if (f==NULL) {
		fault("Can't write to %s\n",tmp);
	}
	fprintf(f,"P6\n%d %d\n15\n",width,height);
	int i;
	for (i=0;i<width*height*2;i+=2) {
		fputc(data[i+1]>>4,f);
		fputc(data[i+1]&0xf,f);
		fputc(data[i]>>4,f);
	}
	fclose(f);
	sprintf(tmp,"%s.%d.alpha.pgm",FileName,n);
	f = fopen(tmp,"wb");
	if (f==NULL) {
		fault("Can't write to %s\n",tmp);
	}
	fprintf(f,"P5\n%d %d\n15\n",width,height);
	for (i=0;i<width*height*2;i+=2) {
		fputc(data[i]&0xf,f);
	}
	fclose(f);
}

static void
decode_texture(struct ZFILE *f, int sz,struct sc_decode *decode) {
//	struct texture * tex = alloc_texture();
	int type = read_byte(f);
	int width = read_word(f);
	int height = read_word(f);
	sz-=5;
	switch (type) {
	case 0:
	case 1: {
		void * data = malloc(width * height * 4);
		read_bytes(f, data, width * height * 4);
//		tex->id = texture_create(data, Texture2DPixelFormat_RGBA8888, width, height);
//		tex->width = 1.0f/width;
//		tex->height = 1.0f/height;
		// 8888
		if (width * height * 4 != sz) {
			fault("Invalid texture %d x %d\n", width, height);
		}
		write_ppm8(data,decode->texture,width,height);
		free(data);
		break;
	}
	case 2: {
		void * data = malloc(width * height * 2);
		read_bytes(f, data, width * height * 2);
//		tex->id = texture_create(data, Texture2DPixelFormat_RGBA4444, width, height);
//		tex->width = 1.0f/width;
//		tex->height = 1.0f/height;
		// 4444
		if (width * height * 2 != sz) {
			fault("Invalid texture %d x %d\n", width, height);
		}
		write_ppm4(data,decode->texture,width,height);
		free(data);
		break;
	}
	}
}

static void
decode_fragment(struct ZFILE *f, struct fragment *frag, int tbase) {
	frag->texid = read_byte(f) + tbase;
	read_bytes(f,(void *)frag->dest, 32);
	read_bytes(f,(void *)frag->src, 16);
	read_bytes(f,(char *)frag->unknown,5);
}

static void
decode_picture(struct ZFILE *f, int sz, struct sc_decode *decode) {
	int id = read_word(f);
	if (id > decode->max_sprite) {
		decode->max_sprite = id;
	}
	id += decode->sprite_base;
	if (id >=MAX_SPRITE || Sprite[id] != NULL) {
		fault("invalid id (base %d : %d) picture\n",decode->sprite_base , id-decode->sprite_base);
	}
	Sprite[id] = malloc(sizeof(struct sprite_res));
	Sprite[id]->type = SPRITE_PICTURE;
	struct picture * pic = &Sprite[id]->u.p;

	pic->n = read_word(f);
	pic->f = malloc(pic->n * sizeof(struct fragment));

	if (sz != 9 + 54 * pic->n) {
		fault("Invalid block. size = %d, fragment = %d\n", sz, pic->n);
	}

	read_byte(f);
	read_dword(f);	// skip unknown 5 bytes

	int i;
	for (i=0;i<pic->n;i++) {
		decode_fragment(f,&pic->f[i],decode->texture_base);
	}
}

/*
static void
dump_bytes(void *buffer, int sz) {
	uint8_t *buf = buffer;
	int i;
	for (i=0;i<sz;i++) {
		printf("%02x ", buf[i]);
		if (i%16 == 15) {
			printf("\n");
		}
	}
	printf("\n");
	for (i=0;i<sz;i++) {
		printf("%c", buf[i]);
	}
	printf("\n");
}
*/

static int
decode_poly(struct ZFILE *f, struct fragment *frag, int tbase) {
	int texid = read_byte(f);
	// todo: why mod 16 ?
	texid %= 16;
	frag->texid = texid + tbase;
	int sz = read_dword(f);
	char buf[sz];
	read_bytes(f,buf,sz);
	int pn = buf[1];
	int i;
	for (i=0;i<4;i++) {
		memcpy(&frag->dest[i].x, buf+2+8*i,4);
		memcpy(&frag->dest[i].y, buf+2+8*i+4,4);
		memcpy(&frag->src[i].x, buf+2+pn*8+4*i,2);
		memcpy(&frag->src[i].y, buf+2+pn*8+4*i+2,2);
	}
	return sz+5;
}


static void
decode_picture_new(struct ZFILE *f, int sz, struct sc_decode *decode) {
	int id = read_word(f);
	if (id > decode->max_sprite) {
		decode->max_sprite = id;
	}
	id += decode->sprite_base;
	if (id >=MAX_SPRITE || Sprite[id] != NULL) {
		fault("invalid id (base %d : %d) picture\n",decode->sprite_base , id-decode->sprite_base);
	}
	Sprite[id] = malloc(sizeof(struct sprite_res));
	Sprite[id]->type = SPRITE_PICTURE;
	struct picture * pic = &Sprite[id]->u.p;

	pic->n = read_word(f);
	pic->f = malloc(pic->n * sizeof(struct fragment));

	// point count : 4/6/8
	read_word(f);
	sz-=6;

	int i;
	for (i=0;i<pic->n;i++) {
		sz -= decode_poly(f,&pic->f[i],decode->texture_base);
	}
	if (sz !=5 ) {
		fault("invalid new picture");
	}

	// skip 5 zero
	read_bytes(f, NULL, 5);
}

static void
decode_animation(struct ZFILE *f, int sz, struct sc_decode *decode) {
	int id = read_word(f);
	if (id > decode->max_sprite) {
		decode->max_sprite = id;
	}
	
	id += decode->sprite_base;
	if (id >= MAX_SPRITE || Sprite[id] != NULL) {
		fault("Invalid id %d\n",id);
	}
	Sprite[id] = malloc(sizeof(struct sprite_res));
	Sprite[id]->type = SPRITE_ANIMATION;
	struct animation * ani = &Sprite[id]->u.a;
	int type = read_byte(f);
	if (type != 0x18) {
		// todo: type mean?
//		printf("Invalid animation type = %d\n", type);
	}
	ani->type = type;
	ani->frame_n = read_word(f);
	ani->part_n = (int)read_dword(f);
	ani->frame = malloc(sizeof(struct animation_frame) * ani->frame_n);
	ani->part = malloc(sizeof(struct animation_part) * ani->part_n);
	int i;
	for (i=0;i<ani->part_n;i++) {
		ani->part[i].index = read_word(f);
		int type8 = read_word(f);
		if (type8 != 0xffff) {
			if (type8 >= decode->offset_count) {
				fault("Invalid id %d in animation frame\n", type8);
			}
			ani->part[i].m = malloc(24);
			memcpy(ani->part[i].m, decode->ot[type8].m, 24);
		} else {
			ani->part[i].m = NULL;
		}
		int alpha = read_word(f);
		if (alpha != 0xffff) {
			if (alpha >= decode->alpha_count) {
				fault("Invalid alpha id %d in animation frame\n", alpha);
			}
			ani->part[i].color = decode->bt[alpha];
		} else {
			ani->part[i].color.mul = 0xffffffff;
			ani->part[i].color.add = 0;
		}

	}
	ani->pic_n = read_word(f);
	ani->pic = malloc(sizeof(struct animation_pic) * ani->pic_n);
	for (i=0;i<ani->pic_n;i++) {
		ani->pic[i].id = read_word(f) + decode->sprite_base;
	}
	for (i=0;i<ani->pic_n;i++) {
		ani->pic[i].attrib = read_byte(f);
	}
	for (i=0;i<ani->pic_n;i++) {
		int slen = read_byte(f);
		if (slen == 0xff) {
			ani->pic[i].name = NULL;
		} else {
			ani->pic[i].name = malloc(slen+1);
			read_bytes(f, ani->pic[i].name, slen);
			ani->pic[i].name[slen] = '\0';
		}
	}

	int index = 0;

	for (i=0;i<ani->frame_n;i++) {
		int at = read_byte(f);
		if (at != 0xb) {
			fault("Unknown animation frame type %d\n", at);
		}
		int len = read_dword(f) - decode->magic;
		int part = read_word(f);
		int strlen = read_byte(f);

		ani->frame[i].index = index;
		ani->frame[i].n = part;
		index += part;

		if (strlen != 0xff) {
			if (strlen + 3 != len) {
				fault("Invalid animation named frame size %d %d\n", strlen, len);
			}
			ani->frame[i].name = malloc(strlen+1);
			read_bytes(f,ani->frame[i].name,strlen);
			ani->frame[i].name[strlen]= '\0';
		} else {
			ani->frame[i].name = NULL;
		}
	}

	read_bytes(f, ani->res, 5);
}

static void
decode_offset(struct ZFILE *f,int sz,struct sc_decode *decode) {
	if (sz != 24) {
		fault("Invalid offset block\n");
	}
	struct offset_table * ot = decode->ot + (decode->offset++);
	read_bytes(f, (char *)ot, 24);
}

static void
decode_alpha(struct ZFILE *f,int sz,struct sc_decode *decode) {
	if (sz != 7) {
		fault("Invalid alpha block\n");
	}

	struct blend_table *bt = &decode->bt[decode->alpha++];
	uint32_t b = read_byte(f);
	uint32_t g = read_byte(f);
	uint32_t r = read_byte(f);
	bt->mul = read_dword(f);
	bt->add = r << 16 | g << 8 | b;
}

static void
decode_text(struct ZFILE *f,int sz,struct sc_decode *decode) {
	int id = read_word(f);
	if (id > decode->max_sprite) {
		decode->max_sprite = id;
	}
	id += decode->sprite_base;
	if (id >=MAX_SPRITE || Sprite[id] != NULL) {
		fault("invalid id (base %d : %d) picture\n",decode->sprite_base , id-decode->sprite_base);
	}
	Sprite[id] = malloc(sizeof(struct sprite_res));
	Sprite[id]->type = SPRITE_TEXT;
	struct text * t = &Sprite[id]->u.t;

	int slen = read_byte(f);
	if (slen == 0xff) {
//		fault("invalid font name \n");
		t->font = malloc(1);
		t->font[0] = '\0';
	} else {
		t->font = malloc(slen+1);
		read_bytes(f, t->font, slen);
		t->font[slen] = '\0';
	}

	t->color = read_dword(f);
	read_bytes(f, (char*)t->res,16);
	t->align = t->res[4];
	t->size = t->res[5];
	t->width = t->res[10] + t->res[11] * 256;
	t->height = t->res[12] + t->res[13] * 256;
}

static int
decode_segment(struct ZFILE *f, struct sc_decode *decode) {
	int type = read_byte(f);
	uint32_t sz = read_dword(f) - decode->magic;
	switch(type) {
	case 1:
		if ((sz & 1) == 0 && decode->magic == 0) {
			decode->magic = 1;
			sz--;
		}
		++decode->texture;
		decode_texture(f, sz, decode);
		break;
	case 0:
		return 1;
	case 18: {
		++decode->picture;
		decode_picture_new(f,sz, decode);
		break;
	}
	case 2: {
		++decode->picture;
		decode_picture(f,sz, decode);
		break;
	}
	case 12: {
		++decode->animation;
		decode_animation(f,sz, decode);
		break;
	}
	case 8: {
		decode_offset(f,sz,decode);
		break;
	}
	case 9: {
		decode_alpha(f,sz,decode);
		break;
	}
	case 7:
		decode_text(f, sz, decode);
		break;
	case 15: {
		decode_text(f, sz, decode);
		// todo: skip one byte, must be 1
		read_byte(f);
		break;
	}
	default:
		printf("Unknown type %d\n", type);
		read_bytes(f, NULL, sz);
		break;
	}
	return 0;
}

static int
compstr(const void *a, const void *b) {
	const struct name *aa = a;
	const struct name *bb = b;
//	return strcmp(aa->str,bb->str);
	return aa->id - bb->id;
}

static const char *
bsearch_name(int id) {
	int begin = 0;
	int end = ExportNumber-1;
	while (begin <= end) {
		int mid = (begin + end)/2;
		struct name * n = &Export[mid];
		if (n->id == id) {
			return n->str;
		} else if (n->id < id) {
			begin = mid + 1;
		} else {
			end = mid -1;
		}
	}
	return NULL;
}

static void
sprite_load(const char * filename) {
	struct ZFILE *f = zfopen(filename);
	if (f == NULL) {
		fault("Can't open %s\n",filename);
	}
	struct sc_decode decode;
	memset(&decode, 0, sizeof(decode));
	decode.texture_base = 0;
	decode.sprite_base = SpriteNumber;

	int pic = read_word(f);
	int ani = read_word(f);
	int tex = read_word(f);
	read_word(f); // type7
	decode.offset_count = read_word(f); 
	decode.offset = 0;
	decode.ot = malloc(decode.offset_count * sizeof(struct offset_table));
	memset(decode.ot, 0, decode.offset_count * sizeof(struct offset_table));

	decode.alpha = 0;
	decode.alpha_count = read_word(f);
	decode.bt = malloc(decode.alpha_count * sizeof(struct blend_table));
	int i;
	for (i=0;i<decode.alpha_count; i++) {
		decode.bt[i].mul = 0xffffffff;
		decode.bt[i].add = 0;
	}

	read_byte(f);	// skip unknown 5 bytes
	read_dword(f);
	int export = read_word(f);
	if (ExportNumber + export >= MAX_EXPORT) {
		fault("Too many export in %s\n",filename);
	}
	for (i=0;i<export;i++) {
		Export[ExportNumber + i].id = read_word(f) + SpriteNumber;
	}
	for (i=0;i<export;i++) {
		int slen = read_byte(f);
		Export[ExportNumber +i].str = malloc(slen+1);
		char * buffer = Export[ExportNumber +i].str;
		read_bytes(f, buffer, slen);
		buffer[slen] = '\0';
	}
	ExportNumber += export;
	qsort(Export, ExportNumber, sizeof(struct name), compstr);
	do {
		if (zfeof(f)) {
			fault("Invalid file %s\n", filename);
		}
		int end = decode_segment(f,&decode);
		if (end) {
			printf("decode.tex = %d tex = %d\n", decode.texture , tex);
			printf("decode.ani = %d ani = %d\n", decode.animation , ani);
			printf("decode.pic = %d pic = %d\n", decode.picture , pic);
			break;
		}
	} while (!(decode.texture == tex && decode.animation == ani && decode.picture == pic));

	SpriteNumber += decode.max_sprite + 1;
	free(decode.ot);
	free(decode.bt);
	TextureNumber = decode.texture;
	zfclose(f);
}

static void
export_picture(FILE *f, struct picture *p, int id) {
	fprintf(f, "picture {\n");
	const char * export = bsearch_name(id);
	if (export) {
		fprintf(f, "\texport = \"%s\",\n", export);
	}
	fprintf(f,"\tid = %d,\n",id);
	int i;
	for (i=0;i<p->n;i++) {
		fprintf(f, "\t{ tex = %d, src = { %d,%d, %d,%d, %d,%d, %d,%d }, screen = { %d,%d, %d,%d, %d,%d, %d,%d }",
			p->f[i].texid,
			(int)p->f[i].src[0].x,(int)p->f[i].src[0].y,
			(int)p->f[i].src[1].x,(int)p->f[i].src[1].y,
			(int)p->f[i].src[2].x,(int)p->f[i].src[2].y,
			(int)p->f[i].src[3].x,(int)p->f[i].src[3].y,
			p->f[i].dest[0].x,p->f[i].dest[0].y,
			p->f[i].dest[1].x,p->f[i].dest[1].y,
			p->f[i].dest[2].x,p->f[i].dest[2].y,
			p->f[i].dest[3].x,p->f[i].dest[3].y);
		if (p->f[i].unknown[0] !=0 ||
			p->f[i].unknown[1] !=0 ||
			p->f[i].unknown[2] !=0 ||
			p->f[i].unknown[3] !=0 ||
			p->f[i].unknown[4] !=0) {
			fprintf(f,", data = '%02x %02x %02x %02x %02x'",
				p->f[i].unknown[0],
				p->f[i].unknown[1],
				p->f[i].unknown[2],
				p->f[i].unknown[3],
				p->f[i].unknown[4]);
		}
		fprintf(f,"},\n");
	}
	fprintf(f,"}\n");
}

static int
is_text(FILE *f, int id) {
	struct sprite_res * res = Sprite[id];
	if (res == NULL || res->type != SPRITE_TEXT) {
		return 0;
	} 
	struct text * t = &res->u.t;
	char data[128];
	sprintf(data, "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
		t->res[0],t->res[1],t->res[2],t->res[3],
		t->res[4],t->res[5],t->res[6],t->res[7],
		t->res[8],t->res[9],t->res[10],t->res[11],
		t->res[12],t->res[13],t->res[14],t->res[15]);
	fprintf(f, "font = \"%s\", color = 0x%x, align = %d, size = %d, width = %d, height = %d, data = '%s' },\n", t->font, t->color,t->align, t->size, t->width, t->height, data);
	return 1;
}

static int
allzero(char *buffer, int sz) {
	int i;
	for (i=0;i<sz;i++) {
		if (buffer[i])
			return 0;
	}
	return 1;
}

static void
export_animation(FILE *f, struct animation *p, int id) {
	fprintf(f, "animation {\n");
	const char * export = bsearch_name(id);
	if (export) {
		fprintf(f, "\texport = \"%s\",\n", export);
	}
	if (p->type != 0x18) {
		fprintf(f,"\ttype = 0x%x,\n",p->type);
	}
	if (!allzero(p->res,5)) {
		fprintf(f,"\tunknown = '%02x %02x %02x %02x %02x',\n",
			p->res[0],p->res[1],p->res[2],p->res[3],p->res[4]);
	}
	fprintf(f,"\tid = %d,\n",id);
	fprintf(f,"\tcomponent = {\n");
	int i;
	for (i=0;i<p->pic_n;i++) {
		if (p->pic[i].name) {
			fprintf(f,"\t\t{ name = \"%s\", ", p->pic[i].name);
			if (!is_text(f, p->pic[i].id)) {
				fprintf(f," id = %d },\n", p->pic[i].id);
			}
		} else {
			fprintf(f,"\t\t{ id = %d },\n", p->pic[i].id);
		}
	}
	fprintf(f,"\t},\n");
	if (p->frame[0].name == NULL) {
		fprintf(f,"\t{\n");
	}
	int first = 0;
	for (i=0;i<p->frame_n;i++) {
		struct animation_frame *frame = &p->frame[i];
		if (frame->name) {
			if (first) {
				fprintf(f,"\t},\n");
			} else {
				first = 1;
			}
			fprintf(f,"\t{ action = \"%s\",\n",frame->name);
		}
		fprintf(f,"\t\t{ ");
		int j;
		for (j=0;j<frame->n;j++) {
			struct animation_part *part = &p->part[frame->index+j];
			if (part->color.mul == 0xffffffff && part->color.add == 0 && part->m == NULL) {
				fprintf(f, "%d, ", part->index);
			} else {
				fprintf(f, "{ index = %d, ", part->index);
				if (part->color.mul != 0xffffffff || part->color.add != 0) {
					fprintf(f, "color = 0x%x,",part->color.mul);
					fprintf(f, "add = 0x%x,",part->color.add);
				}
				if (part->m) {
					fprintf(f, "mat = {%d,%d,%d,%d,%d,%d} ", 
						part->m[0],
						part->m[1],
						part->m[2],
						part->m[3],
						part->m[4],
						part->m[5]
					);
				}
				fprintf(f, "}, ");
			}
		}
		fprintf(f,"},\n");
	}
	fprintf(f,"\t},\n");
	fprintf(f,"}\n");
}

static void
export(FILE *f) {
	int i;
	fprintf(f, "texture(%d)\n", TextureNumber);
	for (i=0;i<SpriteNumber;i++) {
		struct sprite_res * s = Sprite[i];
		if (s==NULL)
			continue;
		switch(s->type) {
		case SPRITE_PICTURE:
			export_picture(f, &s->u.p, i);
			break;
		case SPRITE_ANIMATION:
			export_animation(f, &s->u.a, i);
			break;
		}
	}
}

int
main(int argc, char * argv[]) {
	if (argc <=1 ) {
		return 1;
	}
	FILE *f = fopen(argv[1],"rb");
	if (f==NULL) {
		printf("Can't open %s\n",argv[1]);
		return 1;
	}
	fclose(f);
	FileName = argv[1];
	sprite_load(argv[1]);

	char tmp[strlen(argv[1]) + 8];
	sprintf(tmp, "%s.lua", argv[1]);

	f = fopen(tmp, "wb");
	if (f == NULL) {
		printf("Can't write to %s\n",tmp);
		exit(1);
	}
	export(f);

	fclose(f);

	return 0;
}
