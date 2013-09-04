#include "eploader.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define TYPE_ANIMATION 0
#define TYPE_PICTURE 1
#define TYPE_CLIPUI 2

#define PART_COMPONENT 0
#define PART_SWITCH 1
#define PART_LABEL 2
#define PART_MOUNT 3
#define PART_LABEL_V2 4

#define FRAME_ID 0
#define FRAME_COLOR 1
#define FRAME_MAT 2
#define FRAME_CLIP 4

#ifdef EXPORT_EP

struct alloc {
  int sz;
};

static struct alloc *
init_alloc(int sz) {
  struct alloc * a = malloc(sizeof(*a));
  a->sz = 0;
  return a;
}

static void *
alloc(struct alloc * a, int sz) {
  if (sz & 3) {
    sz = (sz + 3) & ~3;
  }
  a->sz += sz;
  return malloc(sz);
}

#else

struct alloc {
  int sz;
  char * free;
};

static struct alloc *
init_alloc(int sz) {
  struct alloc *a = malloc(sizeof(*a) + sz);
  a->sz = sz;
  a->free = (char *)(a+1);
  return a;
}

static void *
alloc(struct alloc *a, int sz) {
  if (sz & 3) {
    sz = (sz + 3) & ~3;
  }
  assert(sz <= a->sz);
  void * ret = a->free;
  a->sz -= sz;
  a->free += sz;
  return ret;
}

#endif

struct datastream {
  uint8_t * ptr;
  int sz;
};

static int
readbyte(struct datastream *ds) {
  int ret = *(uint8_t*)ds->ptr;
  ds->ptr++;
  ds->sz--;
  return ret;
}

static int
readshort(struct datastream *ds) {
  uint16_t ret = ds->ptr[0] | ds->ptr[1] << 8;
  ds->ptr+=2;
  ds->sz-=2;
  return (int16_t)ret;
}

static int
readuint16(struct datastream *ds) {
  uint16_t ret = ds->ptr[0] | ds->ptr[1] << 8;
  ds->ptr+=2;
  ds->sz-=2;
  return ret;
}

static int
readlong(struct datastream *ds) {
  uint32_t ret = ds->ptr[0] | ds->ptr[1] << 8 | ds->ptr[2] << 16 | ds->ptr[3] << 24;
  ds->ptr+=4;
  ds->sz-=4;
  return (int)ret;
}

static uint32_t
readuint32(struct datastream *ds) {
  uint32_t ret = ds->ptr[0] | ds->ptr[1] << 8 | ds->ptr[2] << 16 | ds->ptr[3] << 24;
  ds->ptr+=4;
  ds->sz-=4;
  return ret;
}

static const char *
readstring(struct ejoypic *ep, struct datastream *ds) {
  int len = readbyte(ds);
  if (len == 0xff)
    return NULL;
  char * ret = alloc(ep->alloc, len+1);
  memcpy(ret, ds->ptr, len);
  ret[len] = '\0';
  ds->ptr += len;
  ds->sz -= len;
  return ret;
}

static void
init_ep(struct ejoypic *ep) {
  ep->export = alloc(ep->alloc, ep->export_n * sizeof(struct export));
  memset(ep->export, 0, ep->export_n * sizeof(struct export));
  ep->export_n = 0;
  ep->spr = alloc(ep->alloc, (ep->max_id+1) * sizeof(struct animation *));
  memset(ep->spr, 0, (ep->max_id+1) * sizeof(struct animation *));
}

static struct picture *
decode_pic(struct ejoypic *ep, struct datastream *ds) {
  int n = readshort(ds);
  struct picture * pic = alloc(ep->alloc, sizeof(*pic) + n * sizeof(struct picture_part));
  assert(n>0);
  pic->n = -n;
  int i,j;
  for (i=0;i<n;i++) {
    struct picture_part * p = &pic->part[i];
    p->texid = readbyte(ds);
    for (j=0;j<8;j++) {
      p->src[j] = readshort(ds);
    }
    for (j=0;j<8;j++) {
      p->screen[j] = readlong(ds);
    }
  }
  return pic;
}

static void
decode_part(struct ejoypic *ep, struct datastream *ds, struct animation_part *part) {
  int type = readbyte(ds);
  switch(type) {
  case PART_COMPONENT:
    part->id = readuint16(ds);
    part->name = NULL;
    part->text = NULL;
    break;
  case PART_SWITCH:
    part->id = readuint16(ds);
    part->name = readstring(ep, ds);
    part->text = NULL;
    break;
  case PART_LABEL:
    part->id = ~0;
    part->name = readstring(ep,ds);
    part->text = alloc(ep->alloc, sizeof(struct label));
    part->text->font = readstring(ep,ds);
    part->text->color = readuint32(ds);
    part->text->size = readbyte(ds);
    part->text->align = readbyte(ds);
    part->text->width = readuint16(ds);
    part->text->height = readuint16(ds);
    break;
  case PART_MOUNT:
    part->id = ~0;
    part->name = readstring(ep,ds);
    part->text = NULL;
    break;
  case PART_LABEL_V2:
    part->id = ~0;
    part->name = readstring(ep,ds);
    part->text = alloc(ep->alloc, sizeof(struct label));
    part->text->font = readstring(ep,ds);
    part->text->color = readuint32(ds);
    part->text->size = readbyte(ds);
    part->text->align = readbyte(ds);
    part->text->width = readuint16(ds);
    part->text->height = readuint16(ds);
    part->text->edge = readbyte(ds);
    break;
  default:
    assert(0);
  }
}

static void
decode_frame(struct ejoypic *ep, struct datastream *ds, struct animation_frame *frame) {
  frame->n = readshort(ds);
  frame->c = alloc(ep->alloc, frame->n * sizeof(struct animation_component));
  int i;
  for (i=0;i<frame->n;i++) {
    int type = readbyte(ds);
    frame->c[i].index = readshort(ds);
    if (type & FRAME_COLOR) {
      frame->c[i].color = readuint32(ds);
      frame->c[i].additive = readuint32(ds);
    } else {
      frame->c[i].color = 0xffffffff;
      frame->c[i].additive = 0;
    }
    if (type & FRAME_MAT) {
      int32_t *mat = alloc(ep->alloc, 6 * sizeof(int));
      frame->c[i].mat = mat;
      int j;
      for (j=0;j<6;j++) {
        mat[j] = readlong(ds);
      }
    } else {
      frame->c[i].mat = NULL;
    }
    if (type & FRAME_CLIP) {
      frame->c[i].flag = 1;
    } else {
      frame->c[i].flag = 0;
    }
  }
}

static void
decode_action(struct ejoypic *ep, struct datastream *ds, struct animation_action *action) {
  action->name = readstring(ep,ds);
  action->n = readshort(ds);
  action->frame = alloc(ep->alloc, action->n * sizeof(struct animation_frame));
  int i;
  for (i=0;i<action->n;i++) {
    decode_frame(ep, ds, &action->frame[i]);
  }
}

static struct animation *
decode_ani(struct ejoypic *ep, struct datastream *ds, int id, int32_t *clipbox) {
  const char * export = readstring(ep, ds);
  if (export) {
    struct export *e = &ep->export[ep->export_n++];
    e->name = export;
    e->id = id;
  }
  int part_n = readshort(ds);
  assert(part_n>=0);
  struct animation_part * parts = alloc(ep->alloc, part_n * sizeof(struct animation_part));
  int i;
  for (i=0;i<part_n;i++) {
    decode_part(ep, ds, &parts[i]);
  }
  int action_n = readshort(ds);
  struct animation * ani = alloc(ep->alloc, sizeof(struct animation) + action_n * sizeof(struct animation_action));
  ani->clipbox = clipbox;
  ani->part_n = part_n;
  ani->part = parts;
  ani->action_n = action_n;
  for (i=0;i<action_n;i++) {
    decode_action(ep, ds, &ani->action[i]);
  }

  return ani;
}

static struct animation *
decode_clipui(struct ejoypic *ep, struct datastream *ds, int id) {
  int32_t *clipbox = alloc(ep->alloc, 4 * sizeof(int32_t));
  clipbox[0] = readlong(ds);
  clipbox[1] = readlong(ds);
  clipbox[2] = readlong(ds);
  clipbox[3] = readlong(ds);
  return decode_ani(ep, ds, id, clipbox);
}

struct ejoypic *
ejoypic_import(void *data, int sz, int cap) {
  struct alloc * a = init_alloc(cap);
  struct ejoypic * ep = alloc(a, sizeof(*ep));
  ep->alloc = a;
  sz -= 2*sizeof(int16_t);
  int16_t * eod = data + sz;
  ep->max_id = eod[0];
  ep->export_n = eod[1];
  init_ep(ep);
  struct datastream ds = { data, sz };
  void * spr = NULL;
  while (ds.sz > 0) {
    int type = readbyte(&ds);
    int id = readuint16(&ds);
    assert(ep->spr[id] == NULL);
    switch (type) {
    case TYPE_ANIMATION:
      spr = decode_ani(ep, &ds, id, NULL);
      break;
    case TYPE_PICTURE:
      spr = decode_pic(ep, &ds);
      break;
    case TYPE_CLIPUI:
      spr = decode_clipui(ep, &ds, id);
      break;
    default:
      free(a);
      return NULL;
    }
    ep->spr[id] = spr;
  }
  return ep;
}

#ifdef EXPORT_EP

#include <lua.h>
#include <lauxlib.h>

static int
lsize(lua_State *L) {
  size_t sz = 0;
  const char * data = luaL_checklstring(L, 1, &sz);
  struct ejoypic * ep = ejoypic_import((void *)data, (int)sz, 0);
  int cap = ep->alloc->sz;
  lua_pushinteger(L, cap);
  return 1;
}

int
luaopen_epconv(lua_State *L) {
  lua_pushcfunction(L, lsize);
  return 1;
};

#endif
