#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ej_memory.h"

// #define __EJ_MEMORY_DEBUG__

#ifdef __EJ_MEMORY_DEBUG__

#include <assert.h>
#define _assert(c) assert(c)
#define _check_tag(t) _assert(0 <= t && t <= MAX_TAG)

#else

#define _assert(c) ((void)0)
#define _check_tag(t) ((void)0)

#endif

/**************************************
raw = [32, 20, 80, 16, 40, 160, 64, 320, 640, 8, 48, 128, 44, 24, 256, 28, 36, 96, 12, 62, 52, 76, 26, 34, 512, 45, 31, 33, 29, 27, 60, 30, 37, 35, 25, 56, 23, 38, 1280, 192, 72]

sizes = set()
for x in raw:
    xx = (x + 3) >> 2
    sizes.add(xx)

x = 4
while x <= 512:
    sizes.add(x >> 2)
    x *= 2

while x <= 4096:
    sizes.add(x >> 2)
    x += 512
**************************************/
static int bsize[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18, 19, 20, 24, 32, 40, 48, 64, 80, 128, 160, 256, 320, 384, 512, 640, 768, 896, 1024 };
// static int bsize[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
#define NBSIZE (sizeof(bsize)/sizeof(bsize[0]))

#define MEMBLOCK_SIZE (1024*1024) /* 1MB */

#define SIZE2TAG(x) (((x+3)>>2)-1)
#define TAG2SIZE(x) ((x+1)<<2)
#define MAX_PIECE_SIZE (0x1000) /* 4096 */
#define MAX_TAG SIZE2TAG(MAX_PIECE_SIZE) /* 0x400 1024 */

#define MemPieceBase struct mempiece_t *next

typedef struct mempiece_t {
  MemPieceBase;
} mempiece_t;

typedef struct mempiece_head_t {
  MemPieceBase;
  size_t size;
} mempiece_head_t;

typedef struct memblock_t {
  struct memblock_t *next;
} memblock_t;

struct ej_memory_t {
  mempiece_head_t *all_pieces;
  mempiece_head_t *buckets[MAX_TAG+1]; /* 0x1000 >> 2 */

  memblock_t *all_mblk;
  memblock_t *cur_mblk;
  size_t cur_offset;

  // stat
  size_t mblk_count;
  size_t wild_malloc_count;
  size_t tame_malloc_count;
  size_t wasted_bytes;
};


ej_memory_t *
ejoy_memory_create() {
  int i;
  ej_memory_t *m = (ej_memory_t *)malloc(sizeof(*m));
  memset(m, 0, sizeof(*m));

  m->all_pieces = (mempiece_head_t *)malloc(sizeof(mempiece_head_t) * NBSIZE);
  memset(m->all_pieces, 0, sizeof(mempiece_head_t) * NBSIZE);

  for(i=0; i<NBSIZE; ++i) {
    size_t size = bsize[i];
    m->buckets[size-1] = m->all_pieces + i;
    size_t sz = size;
    int j;
    for(j=i+1; j<NBSIZE && (sz<<2)<sizeof(mempiece_t); j++) {
      sz = bsize[j];
    }
    m->buckets[size-1]->size = (sz<<2);
  }
  _assert(m->buckets[MAX_TAG]);
  mempiece_head_t *p = NULL;
  for(i=MAX_TAG; i>=0; --i) {
    if(m->buckets[i]) {
      p = m->buckets[i];
    } else {
      m->buckets[i] = p;
    }
  }

  return m;
}

void
ejoy_memory_free(ej_memory_t *m) {
  if(m) {
    free(m->all_pieces);

    memblock_t *mblk = m->all_mblk;
    while(mblk) {
      void *tmp = (void*)mblk;
      mblk = mblk->next;
      free(tmp);
    }

    free(m);
  }
}

void ejoy_memory_report(ej_memory_t *m) {
  printf("[ej_memory] blk count: %zu\n", m->mblk_count);
  printf("[ej_memory] wild/total: %f (%zu/%zu)\n",
    (double)m->wild_malloc_count /
      (double)(m->wild_malloc_count + m->tame_malloc_count),
    m->wild_malloc_count, m->tame_malloc_count);
  printf("[ej_memory] wasted bytes: %zu\n", m->wasted_bytes);
}

static void
_free(mempiece_head_t *head, void *ptr) {
  mempiece_t *p = (mempiece_t *)ptr;
  p->next = head->next;
  head->next = p;
}

static mempiece_head_t *
_find_bucket_le(ej_memory_t *m, size_t size) {
  if(size < sizeof(mempiece_t)) {
    return NULL;
  }

  int tag = SIZE2TAG(size);
  _check_tag(tag);
  while(tag >= 0) {
    mempiece_head_t *head = m->buckets[tag--];
    if(head->size <= size) {
      return head;
    }
  }
  return NULL;
}

static size_t
_free_rest(ej_memory_t *m, memblock_t *mblk, size_t rest_size) {
  mempiece_head_t *ohead = _find_bucket_le(m, rest_size);
  while(ohead) {
    // printf("ohead->size: %lu\n", ohead->size);
    _free(ohead, (void*)((int8_t *)mblk + m->cur_offset));
    _assert(ohead->size <= rest_size);

    m->cur_offset += ohead->size;
    rest_size -= ohead->size;
    ohead = _find_bucket_le(m, rest_size);
  }
  return rest_size;
}

static void *
_malloc(ej_memory_t *m, mempiece_head_t *head, size_t ntag) {
  size_t nsize = head->size;
  _assert(nsize >= sizeof(mempiece_t));

  if(head->next) {
    mempiece_t *ret = head->next;
    head->next = ret->next;
    return ret;
  }

  memblock_t *mblk = m->cur_mblk;
  if(!mblk || MEMBLOCK_SIZE - m->cur_offset < nsize) {
    m->mblk_count++;
    if(mblk) {
      size_t rest_size = MEMBLOCK_SIZE - m->cur_offset;
      // printf("rest_size: %lu\n", rest_size);
      rest_size = _free_rest(m, mblk, rest_size);
      // printf("waste: %lu\n", rest_size);
      m->wasted_bytes += rest_size;
    }

    mblk = (memblock_t *)malloc(MEMBLOCK_SIZE);
    m->cur_mblk = mblk;
    m->cur_offset = sizeof(memblock_t);
    mblk->next = m->all_mblk;
    m->all_mblk = mblk;
  }

  void *ptr = (void *)((int8_t *)mblk + m->cur_offset);
  m->cur_offset += nsize;
  return ptr;
}

void *
ejoy_malloc(ej_memory_t *m, size_t nsize) {
  if(nsize > MAX_PIECE_SIZE) {
    m->wild_malloc_count++;
    return malloc(nsize);
  }
  if(nsize == 0) {
    return NULL;
  }

  size_t ntag = SIZE2TAG(nsize);
  _check_tag(ntag);
  mempiece_head_t *nhead = m->buckets[ntag];
  m->tame_malloc_count++;
  return _malloc(m, nhead, ntag);
}

void
ejoy_free(ej_memory_t *m, void *ptr, size_t osize) {
  if(ptr == NULL || osize == 0) {
    return;
  }
  if(osize > MAX_PIECE_SIZE) {
    free(ptr);
    return;
  }

  size_t otag = SIZE2TAG(osize);
  _check_tag(otag);
  mempiece_head_t *ohead = m->buckets[otag];
  _free(ohead, ptr);
}

void *
ejoy_realloc(ej_memory_t *m, void *ptr, size_t osize, size_t nsize) {
  if(ptr == NULL) {
    return ejoy_malloc(m, nsize);
  }
  if(osize == nsize) {
    return ptr;
  }
  if(nsize == 0) {
    ejoy_free(m, ptr, osize);
    return NULL;
  }

  void *ptr2 = ejoy_malloc(m, nsize);
  if(ptr2) {
    memcpy(ptr2, ptr, ((osize < nsize) ? osize : nsize));
  }
  ejoy_free(m, ptr, osize);
  return ptr2;
}

//void *
//ejoy_realloc_(ej_memory_t *m, void *ptr, size_t osize, size_t nsize) {
//  if(ptr == NULL) {
//    return ejoy_malloc(m, nsize);
//  }
//  if(osize == nsize) {
//    return ptr;
//  }
//  if(nsize == 0) {
//    ejoy_free(m, ptr, osize);
//    return NULL;
//  }
//
//  if(osize > MAX_PIECE_SIZE && nsize > MAX_PIECE_SIZE) {
//    m->wild_malloc_count++;
//    return realloc(ptr, nsize);
//  }
//
//  mempiece_head_t *ohead = NULL;
//  mempiece_head_t *nhead = NULL;
//  size_t otag, ntag;
//  if(osize <= MAX_PIECE_SIZE) {
//    otag = SIZE2TAG(osize); _check_tag(otag);
//    ohead = m->buckets[otag];
//  }
//  if(nsize <= MAX_PIECE_SIZE) {
//    ntag = SIZE2TAG(nsize); _check_tag(ntag);
//    nhead = m->buckets[ntag];
//  }
//  if(ohead == nhead) {
//    return ptr;
//  }
//
//  void *ptr2 = (nhead ? _malloc(m, nhead, ntag) : malloc(nsize));
//  if(ptr2) {
//    memcpy(ptr2, ptr, ((osize < nsize) ? osize : nsize));
//  }
//  if(ptr2 || nsize == 0) {
//    if(ohead) {
//      _free(ohead, ptr);
//    } else {
//      free(ptr);
//    }
//  }
//  return ptr2;
//}
