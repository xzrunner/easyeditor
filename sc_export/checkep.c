#include "eploader.h"
#include "LzmaDec.h"
#include "Alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TEXTURE4 0
#define TEXTURE8 1
#define DETAIL 2
#define PVRTC 3

#define LZMA_PROPS_SIZE 5

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

static 
int LzmaUncompress(unsigned char *dest, size_t  *destLen, const unsigned char *src, size_t  *srcLen,
  const unsigned char *props, size_t propsSize)
{
  ELzmaStatus status;
  return LzmaDecode(dest, destLen, src, srcLen, props, (unsigned)propsSize, LZMA_FINISH_ANY, &status, &g_Alloc);
}

struct block {
	uint8_t size[4];
	uint8_t prop[LZMA_PROPS_SIZE];
	uint8_t data[119];
};

static void
check_pvrtc(uint8_t* buffer, size_t sz){
	int internal_format = buffer[0];
	int w = buffer[1] | buffer[2]<<8;
	int h = buffer[3] | buffer[4]<<8;
	printf("----Texure PVRTC format: %d  %dx%d\n", internal_format, w,h);
	int i;
	uint8_t* data = buffer+5;
	uint8_t* head = data;
	for(i=0; head-data < sz-5; i++){
		int size = head[0] | head[1]<<8 | head[2]<<16 | head[3]<<24;
		printf("image data[%d] size:%d\n", i, size);
		head += 4+size;
	}

	if(head-data != sz-5){
		printf("error image data!\n");
	}
}

static void
check_tex4(uint8_t *buffer, size_t sz) {
	int w = buffer[0] | buffer[1]<<8;
	int h = buffer[2] | buffer[3]<<8;
	printf("Texure 4444 %dx%d\n",w,h);
	if (sz-4 != w*h*2) {
		printf("Invalid size %lu\n",sz-4);
	}
}

static void
check_tex8(uint8_t *buffer, size_t sz) {
	int w = buffer[0] | buffer[1]<<8;
	int h = buffer[2] | buffer[3]<<8;
	printf("Texure 8888 %dx%d\n",w,h);
	if (sz-4 != w*h*4) {
		printf("Invalid size %lu\n",sz-4);
	}
}

static void
check_pack(uint8_t *buffer, size_t sz) {
	uint32_t cap = buffer[0] | buffer[1]<<8 | buffer[2]<<16 | buffer[3]<<24;
	printf("%d bytes detail\n",(int)cap);
	struct ejoypic * ep = ejoypic_import(buffer+4, sz - 4, cap);
	if (ep == NULL) {
		printf("Error in detail\n");
	}
	free(ep);
}

static void
checkbuffer(uint8_t * buffer, size_t sz) {
	int type = buffer[0];
	switch(type) {
	case TEXTURE4:
		check_tex4(buffer+1, sz-1);
		break;
	case TEXTURE8:
		check_tex8(buffer+1, sz-1);
		break;
	case PVRTC:
		check_pvrtc(buffer+1, sz-1);
		break;
	case DETAIL:
		check_pack(buffer+1, sz-1);
		break;
	default:
		printf("Invalid section type %d\n",type);
		exit(1);
	}
}

static void
checkblock(FILE *f) {
	int32_t sz = 0;
	if (fread(&sz, sizeof(sz), 1, f) == 0) {
		return;
	}
	unsigned char * buffer;
	if (sz<0) {
		buffer = malloc(-sz);
		if (fread(buffer, -sz, 1, f) != 1) {
			printf("Invalid uncompress data\n");
			exit(1);
		}
		checkbuffer(buffer, -sz);
	} else {
		struct block * block = malloc(sz);
		if (fread(block, sz, 1, f) != 1) {
			printf("Invalid compress data source\n");
			exit(1);
		}

		if (sz <= 4 + LZMA_PROPS_SIZE) {
			printf("Invalid compress data source\n");
			exit(1);
		}
		size_t text_size = block->size[0] << 24 | block->size[1] << 16 | block->size[2] << 8 | block->size[3];
		buffer = malloc(text_size);
		size_t src_sz = sz - sizeof(block->size) - LZMA_PROPS_SIZE;
		int r = LzmaUncompress(buffer, &text_size, block->data, &src_sz, block->prop, LZMA_PROPS_SIZE);
		if (r != SZ_OK) {
			printf("Uncompress error %d\n",r);
			exit(1);
			return;
		}
		free(block);

		checkbuffer(buffer, text_size);
	}
	free(buffer);
}

static void
check(FILE *f) {
	while(!feof(f)) {
		checkblock(f);
	}
}

int
main(int argc, char *argv[]) {
	if (argc == 1)
		return 1;
	FILE *f = fopen(argv[1],"rb");
	if (f==NULL) {
		printf("Can't open %s\n",argv[1]);
		return 1;
	}
	check(f);
	fclose(f);

	return 0;
}
