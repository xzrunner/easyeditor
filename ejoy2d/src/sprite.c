#include "sprite.h"
#include "ejoy2d.h"
#include "package.h"
#include "shader.h"
#include "fault.h"

#include "dtex_facade.h"

#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

#define MAX_CLIPBOX 16

//#define MAX_SCALE 1.25f

struct clipbox {
  int x;
  int y;
  int width;
  int height;
};

// todo
struct dtex_texture;
struct dp_position;

static struct clipbox G_CLIP[MAX_CLIPBOX];
static int G_CLIP_COUNT = 0;

#ifdef USE_DTEX
extern struct dtex* Dtex;
#endif

struct screen_coord *
sprite_default_coord(struct screen_coord * screen) {
  struct ej_screen scr;
  ejoy_get_screen(&scr);
  screen->hscale = 2.0f / SCALE / scr.h;
  screen->wscale = 2.0f / SCALE / scr.w;
  screen->xoffset = 0;
  screen->yoffset = 0;
  screen->inv_scale = 1.0f;

  return screen;
}

struct screen_coord *
sprite_coord(struct screen_coord * screen, int x, int y, float scale) {
  struct ej_screen scr;
  ejoy_get_screen(&scr);
  screen->inv_scale = 1.0f/scale;
  scale*=SCALE;
  x*=SCALE;
  y*=SCALE;
  screen->hscale = 2.0f / scale / scr.h;
  screen->wscale = 2.0f / scale / scr.w;
  screen->xoffset = x;
  screen->yoffset = y;

  return screen;
}

/*
                         SCREEN_WIDTH
               +-------------------------------+   -
               |                               |   ^
               |                               |   y
               |                               |
               |              width            |   v
               |         +--------------+      |   -
               |         |              |      |
SCREEN_HEIGHT  |  height |              |      |
               |         |              |      |
               |         |              |      |
               |         |              |      |
               |         |              |      |
               |         +--------------+      |
               |                               |
               +-------------------------------+
               |<-- x -->|
*/

static void
_trans(int *xx, int *yy, int *width, int *height, int mat[6]) {
    int x1 = -*xx;
    int y1 = -*yy;
    int x2 = *width + x1;
    int y2 = *height + y1;
    x1 *= SCALE;
    y1 *= SCALE;
    x2 *= SCALE;
    y2 *= SCALE;
    
    assert(mat[1] == 0 && mat[2] == 0);
    
    int xx1 = (x1 * mat[0]) / 1024 + mat[4];
    int yy1 = (y1 * mat[3]) / 1024 + mat[5];
    
    int xx2 = (x2 * mat[0]) / 1024 + mat[4];
    int yy2 = (y2 * mat[3]) / 1024 + mat[5];
    
    *xx = -xx1 / SCALE;
    *yy = -yy1 / SCALE;
    *width = (xx2 - xx1) / SCALE;
    *height = (yy2 - yy1) / SCALE;
}

//static void
//_trans(int *xx, int *yy, int *width, int *height, int mat[6]) {
//    int x1 = *xx;
//    int y1 = -*yy;
//    int x2 = *width + x1;
//    int y2 = *height + y1;
//    x1 *= SCALE;
//    y1 *= SCALE;
//    x2 *= SCALE;
//    y2 *= SCALE;
//    
//    assert(mat[1] == 0 && mat[2] == 0);
//    
//    int xx1 = (x1 * mat[0]) / 1024 + mat[4];
//    int yy1 = (y1 * mat[3]) / 1024 + mat[5];
//    
//    int xx2 = (x2 * mat[0]) / 1024 + mat[4];
//    int yy2 = (y2 * mat[3]) / 1024 + mat[5];
//    
//    *xx = xx1 / SCALE;
//    *yy = -yy1 / SCALE;
//    *width = (xx2 - xx1) / SCALE;
//    *height = (yy2 - yy1) / SCALE;
//}

void
screen_clip_push(int *mat, int dx, int dy, int * clipbox, float scale, int test) {
  struct ej_screen scr;
  ejoy_get_screen(&scr);
  assert(G_CLIP_COUNT < MAX_CLIPBOX);
  int x = clipbox[2];
  int y = clipbox[3];
  int width  = clipbox[0];
  int height = clipbox[1];
  float inv_scale = 1.0f / scale;
  if (mat) {
    _trans(&x,&y,&width,&height,mat);
  }
  x += dx / SCALE;
  y += dy / SCALE;
  x *= inv_scale;
  y *= inv_scale;
  width *= inv_scale;
  height *= inv_scale;
  if (x < 0) {
    width +=x;
    x = 0;
  } else if (x >= scr.w) {
     x = scr.w;
     width = 0;
  }
  if (y < 0) {
    height +=y;
    y = 0;
  } else if (y >= scr.h) {
    y = scr.h;
    height = 0;
  }
  if (width < 0) {
    width = 0;
  } else if (x + width > scr.w) {
    width = scr.w - x;
  }
  if (height < 0) {
    height = 0;
  } else if (y + height > scr.h) {
    height = scr.h - y;
  }

  struct clipbox *cb = &G_CLIP[G_CLIP_COUNT++];
  cb->x = x;
  cb->y = y;
  cb->width = width;
  cb->height = height;

  if (!test) {
    shader_scissor(x,y,width,height,1);
  }
}

void
screen_clip_pop(int test) {
  --G_CLIP_COUNT;
  assert(G_CLIP_COUNT>=0);
  if (!test) {
    if (G_CLIP_COUNT == 0) {
      shader_scissor(0,0,0,0,0);
    } else {
      struct clipbox *cb = &G_CLIP[G_CLIP_COUNT-1];
      shader_scissor(cb->x,cb->y,cb->width,cb->height,1);
    }
  }
}

int
screen_clip_test(int x, int y) {
  x /= SCALE;
  y /= SCALE;
  if (G_CLIP_COUNT == 0) {
    struct ej_screen scr;
    ejoy_get_screen(&scr);
    return x>=0 && x<scr.w && y>=0 && y<=scr.h;
  }
  struct clipbox *cb = &G_CLIP[G_CLIP_COUNT-1];
  return x>=cb->x && x<cb->width+cb->x && y>=cb->y && y<=cb->y+cb->height;
}

int
screen_clip_test_rect(int x1, int y1, int x2, int y2) {
  struct rect {
    int left, top, right, bottom;
  } r1, r2;

  x1 /= SCALE; y1 /= SCALE;
  x2 /= SCALE; y2 /= SCALE;

  if(x1 < x2) {
    r1.left  = x1;
    r1.right = x2;
  } else {
    r1.left  = x2;
    r1.right = x1;
  }

  if(y1 < y2) {
    r1.top = y1;
    r1.bottom = y2;
  } else {
    r1.top = y2;
    r1.bottom = y1;
  }

  if (G_CLIP_COUNT == 0) {
    struct ej_screen scr;
    ejoy_get_screen(&scr);
    r2.left   = 0;
    r2.top    = 0;
    r2.right  = scr.w;
    r2.bottom = scr.h;
  }
  else {
    struct clipbox *cb = &G_CLIP[G_CLIP_COUNT-1];
    r2.left   = cb->x;
    r2.top    = cb->y;
    r2.right  = cb->x + cb->width;
    r2.bottom = cb->y + cb->height;
  }

  return ((r1.right >= r2.left && r1.left < r2.right)   // x轴相交
       && (r1.bottom >= r2.top && r1.top < r2.bottom)); // 并且y轴也相交
}

int
sprite_id(struct ej_package *pack, const char *name) {
  struct ejoypic * ep = pack->ep;
  int start = 0;
  int end = ep->export_n-1;
  while (start <= end) {
    int mid = (start + end)/2;
    int r = strcmp(name, ep->export[mid].name);
    if (r == 0) {
      return ep->export[mid].id;
    } else if (r<0) {
      end = mid-1;
    } else {
      start = mid +1;
    }
  }
  return -1;
}

static int
_sprite_size(struct ejoypic *ep, int id) {
  if (id < 0 || ep->max_id < id)
    return sizeof(struct ej_sprite);
  struct animation * ani = ep->spr[id];
  if (ani == NULL)
    return sizeof(struct ej_sprite);
  if (ani->part_n <= 0) {
    return sizeof(struct ej_sprite);
  }
  int sz = sizeof(struct ej_sprite) + ani->part_n * sizeof(struct ej_sprite *);
  int i;
  for (i=0;i<ani->part_n;i++) {
    struct animation_part * part = &ani->part[i];
    if (part->name == NULL) {
      continue;
    }
    if (part->text) {
      sz += sizeof(struct ej_sprite);
    } else {
      sz += _sprite_size(ep, part->id);
    }
  }
  return sz;
}

int
sprite_size(struct ej_package *pack, int id) {
  struct ejoypic * ep = pack->ep;
  return _sprite_size(ep, id);
}

static int
_sprite_fill(struct ej_sprite * spr, struct ej_package *pack, int id) {
  struct ejoypic * ep = pack->ep;
  if (id < 0 || ep->max_id < id)
    return 0;

  struct animation * ani = ep->spr[id];
  if (ani == NULL)
    return 0;
  spr->pack = pack;
  spr->ani = ani;
  spr->frame = 0;
  spr->extra = 0;
  spr->action = 0;
  spr->flag = 0;
  spr->color_trans = 0xffffffff;
  spr->color_additive = 0;

  if (ani->part_n <=0) {
    return sizeof(struct ej_sprite);
  }
  int sz = sizeof(struct ej_sprite) + ani->part_n * sizeof(struct ej_sprite *);
  uintptr_t ns = (uintptr_t)spr + sz;
  int i;
  for (i=0;i<ani->part_n;i++) {
    struct animation_part * part = &ani->part[i];
    if (part->name == NULL) {
      spr->c[i] = NULL;
      continue;
    }
    spr->c[i] = (struct ej_sprite *)ns;
    int s;
    if (part->text) {
      struct ej_sprite * t = spr->c[i];
      t->pack = pack;
      t->ani = NULL;
      t->extra = 0;
      t->frame = 0;
      t->action = 0;
      t->flag = 0;
      t->color_trans = 0xffffffff;
      t->color_additive = 0;
      s = sizeof(struct ej_sprite);
    } else {
      s = _sprite_fill(spr->c[i], pack, part->id);
      if(s == 0) {
        struct ej_sprite * t = spr->c[i];
        t->pack = pack;
        t->ani = NULL;
        t->extra = 0;
        t->frame = 0;
        t->action = 0;
        t->flag = 0;
        t->color_trans = 0xffffffff;
        t->color_additive = 0;
        s = sizeof(struct ej_sprite);
      }
    }
    ns += s;
    sz += s;
  }
  return sz;
}

int
sprite_create(struct ej_sprite * spr, struct ej_package *pack, int id) {
// #ifdef USE_DTEX      
//   dtex_preload_id(Dtex, pack, id, 1);
// #endif // USE_DTEX        
  return _sprite_fill(spr, pack, id);
}

int
sprite_find(struct ej_sprite *spr, struct ej_sprite *node, char *buf, int cap) {
  if (cap == 0)
    return -1;
  struct animation * ani = spr->ani;
  if (ani->part_n <=0) {
    buf[0] = 0;
    return 0;
  }
  int i;
  for (i=0;i<ani->part_n;i++) {
    struct animation_part * part = &ani->part[i];
    if (part->name == NULL)
      continue;

    struct ej_sprite * subs = spr->c[i];
    assert(subs != NULL);
    if (subs->ani == NULL)
      continue;

    int len  = strlen(part->name);
    if (len+1 >= cap)
      return -1;
    memcpy(buf, part->name, len);
    if (subs == node) {
      buf[len] = 0;
      return len;
    }
    buf[len] = '.';
    int n = sprite_find(subs, node, buf+len+1, cap-len-1);
    if (n < 0) {
      return -1;
    }
    if (n > 0) {
      return len + n + 1;
    }
  }

  return 0;
}

// draw

static void
draw(struct ej_package *pack, int x, int y, int m[6], struct picture_part *rect, int mirror, struct screen_coord * screen,
  unsigned int color, unsigned int additive, int shader) {
  assert(pack->texture_n > rect->texid);
  struct texture * t = &pack->tex[rect->texid];

  int xmin, ymin, xmax, ymax;
  xmin = ymin = INT_MAX;
  xmax = ymax = INT_MIN;
  float vb[16];
  int i;
  if (m) {
    for (i=0;i<4;i++) {
      int xx = rect->screen[i*2];
      int yy = rect->screen[i*2+1];
      int dx = (xx * m[0] + yy * m[2]) / 1024 + m[4];
      int dy = (xx * m[1] + yy * m[3]) / 1024 + m[5];
      if (mirror)
        dx = -dx;
      int rx = x + dx - screen->xoffset ;
      int ry = y + dy - screen->yoffset ;
      if (rx < xmin) xmin = rx;
      if (rx > xmax) xmax = rx;
      if (ry < ymin) ymin = ry;
      if (ry > ymax) ymax = ry;
      vb[i*4+0]= screen->wscale * rx - 1.0f;
      vb[i*4+1]= - screen->hscale * ry + 1.0f;
      vb[i*4+2] = t->width * rect->src[i*2];
      vb[i*4+3] = t->height * rect->src[i*2+1];
    }
  } else {
    for (i=0;i<4;i++) {
      int xx = rect->screen[i*2];
      int yy = rect->screen[i*2+1];
      if (mirror)
        xx = -xx;
      int rx = x + xx - screen->xoffset;
      int ry = y + yy - screen->yoffset;
      if (rx < xmin) xmin = rx;
      if (rx > xmax) xmax = rx;
      if (ry < ymin) ymin = ry;
      if (ry > ymax) ymax = ry;
      vb[i*4+0]= screen->wscale * rx - 1.0f;
      vb[i*4+1]= - screen->hscale * ry + 1.0f;
      vb[i*4+2] = t->width * rect->src[i*2];
      vb[i*4+3] = t->height * rect->src[i*2+1];
    }
  }

  if (vb[0] < -1 && vb[4] < -1 && vb[8] < -1 && vb[12] < -1)
    return;
  if (vb[0] > 1 && vb[4] > 1 && vb[8] > 1 && vb[12] > 1)
    return;
  if (vb[1] < -1 && vb[5] < -1 && vb[9] < -1 && vb[13] < -1)
    return;
  if (vb[1] > 1 && vb[5] > 1 && vb[9] > 1 && vb[13] > 1)
    return;

  int TOL = 4 * SCALE;
  if (xmax - xmin < TOL && ymax - ymin < TOL)
    return;

  int texid = t->id;
  int alphaid = t->id_alpha;
// #ifdef USE_DTEX
//   const struct dtex_rect* dr = dtex_lookup(Dtex,vb,texid,1/t->width,1/t->height);
//   // if (dr && screen->inv_scale/dr->scale<dtex_max_scale(Dtex)) {
//   if (dr) {  
//     memcpy(vb+2, dr->vb, 2*sizeof(float));
//     memcpy(vb+6, dr->vb+2, 2*sizeof(float));
//     memcpy(vb+10, dr->vb+4, 2*sizeof(float));
//     memcpy(vb+14, dr->vb+6, 2*sizeof(float));
//     texid = dtex_texid(Dtex);
//     alphaid = 0;
//     if (shader == PROGRAM_SPRITE_KTX || shader == PROGRAM_SPRITE_PKM) {
//       shader = PROGRAM_SPRITE;
//     } else if (shader == PROGRAM_GRAY_KTX || shader == PROGRAM_GRAY_PKM) {
//       shader = PROGRAM_GRAY;
//     }
//   }
// #endif // USE_DTEX

  int new_texid = 0;
  float* tex_vb = dtexf_c2_lookup_texcoords(t, vb, &new_texid);
  if (tex_vb != NULL) {
    // pts
    if (rect->src[0] < 0) {
      struct dtex_texture* tex = NULL;
      struct dp_position* pos = NULL;
      dtexf_c2_lookup_node(t, vb, &tex, &pos);
      if (tex != NULL && pos != NULL) {
        struct draw_params params;
        params.x = x;
        params.y = y;
        params.mat = m;
        params.screen = screen;
        params.color = color;
        params.additive = additive;
        if (dtexf_draw_pts(pack, tex, -rect->src[0], pos, &params, rect->screen)) {
            return;
        }
      }
    }
    memcpy(vb+2, tex_vb, 2*sizeof(float));
    memcpy(vb+6, tex_vb+2, 2*sizeof(float));
    memcpy(vb+10, tex_vb+4, 2*sizeof(float));
    memcpy(vb+14, tex_vb+6, 2*sizeof(float));  
    texid = new_texid;
  } else if (rect->src[0] < 0) {
      struct draw_params params;
      params.x = x;
      params.y = y;
      params.mat = m;
      params.screen = screen;
      params.color = color;
      params.additive = additive;
      dtexf_draw_rrp(pack, t, -rect->src[0], &params, rect->screen);
      return;
  } else {
    // return;
  }

  shader_program(shader);
  shader_color(color,additive);   
  if (shader == PROGRAM_SPRITE_PKM || shader == PROGRAM_GRAY_PKM) {
    shader_draw_separate(vb,texid,alphaid);
  } else {
    shader_draw(vb,texid);
  } 
}

void
sprite_draw(struct ej_package * pack, struct picture * pic, int x, int y, int *mat, unsigned int color, unsigned int additive, int mirror, int shader, struct screen_coord *screen) {
  assert(pic->n < 0);
  int i;
  for (i=0;i<-pic->n;i++) {
    int texid = pic->part[i].texid;
    assert(pack->texture_n > texid);
    struct texture * t = &pack->tex[texid];
    if (t->format == KTX) {
      if (shader == PROGRAM_GRAY) {
        shader = PROGRAM_GRAY_KTX;
      } else {
        shader = PROGRAM_SPRITE_KTX;
      }
    }
    if (t->format == PKM) {
      if (shader == PROGRAM_GRAY) {
        shader = PROGRAM_GRAY_PKM;
      } else {
        shader = PROGRAM_SPRITE_PKM;
      }
    }    
    draw(pack, x,y, mat, &pic->part[i], mirror, screen, color, additive, shader);
  }
}

static void
_get_region(int32_t vb[8],int x,int y,int *m,int offx,int offy,float scale,int mirror,struct ej_rect *rect)
{
  int i;
  if (m) {
    for (i=1;i<4;i++) {
      int xx = vb[i*2];
      int yy = vb[i*2+1];
      int dx = (xx * m[0] + yy * m[2]) / 1024 + m[4];
      int dy = (xx * m[1] + yy * m[3]) / 1024 + m[5];
      if(mirror)
        dx = -dx;
      int rx = (x + dx - offx) / scale;
      int ry = (y + dy - offy) / scale;
      if(rx < rect->xmin) {
        rect->xmin = rx;
      }
      if(rx > rect->xmax) {
        rect->xmax = rx;
      }
      if(ry < rect->ymin) {
        rect->ymin = ry;
      }
      if(ry > rect->ymax) {
        rect->ymax = ry;
      }
    }
  } else {
    for (i=1;i<4;i++) {
      int xx = vb[i*2];
      int yy = vb[i*2+1];
      if(mirror)
        xx = -xx;
      int rx = (x + xx - offx) / scale;
      int ry = (y + yy - offy) / scale;
      if(rx < rect->xmin) {
        rect->xmin = rx;
      }
      if(rx > rect->xmax) {
        rect->xmax = rx;
      }
      if(ry < rect->ymin) {
        rect->ymin = ry;
      }
      if(ry > rect->ymax) {
        rect->ymax = ry;
      }
    }
  }
  //// printf("x:%d (%d,%d) y:%d (%d,%d)\n",c->x,rect->xmin,rect->xmax,c->y,rect->ymin,rect->ymax);
  //int width = rect->xmax - rect->xmin;
  //int height = rect->ymax - rect->ymin;
  //if(width < MIN_TEST_LENGTH * 2) {
  //  int cx = (rect->xmin + rect->xmax) / 2;
  //  rect->xmin = cx - MIN_TEST_LENGTH;
  //  rect->xmax = cx + MIN_TEST_LENGTH;
  //}
  //if(height < MIN_TEST_LENGTH * 2) {
  //  int cy = (rect->ymin + rect->ymax) / 2;
  //  rect->ymin = cy - MIN_TEST_LENGTH;
  //  rect->ymax = cy + MIN_TEST_LENGTH;
  //}  
}

//#define MIN_TEST_LENGTH (192)
#define CLICK_EXTEND (256)

static int
rect_in(int32_t vb[8],int x,int y,int *m,struct ej_cursor *c,int offx,int offy,float scale,int mirror,int extend) {
  struct ej_rect rect;
  rect.xmin = rect.ymin = INT_MAX;
  rect.xmax = rect.ymax = INT_MIN;
  _get_region(vb,x,y,m,offx,offy,scale,mirror,&rect);
  if(extend) {
    return rect.xmin <= c->x + CLICK_EXTEND && c->x - CLICK_EXTEND <= rect.xmax
        && rect.ymin <= c->y + CLICK_EXTEND && c->y - CLICK_EXTEND <= rect.ymax;
  }
  //if (c->x >= rect.xmin && c->x <= rect.xmax && c->y >= rect.ymin && c->y <= rect.ymax) {
  //  printf("x:%d (%d,%d) y:%d (%d,%d) scale:%f\n",c->x,rect.xmin,rect.xmax,c->y,rect.ymin,rect.ymax, scale);
  //}
  return c->x >= rect.xmin && c->x <= rect.xmax && c->y >= rect.ymin && c->y <= rect.ymax;
}

#undef CLICK_EXTEND
//#undef MIN_TEST_LENGTH

static int
test(struct ej_package *pack, int x, int y, int *m, struct picture_part *rect, struct ej_cursor *c, int offx, int offy, float scale, int mirror, int extend) {
  // int px = (c->x *scale) + offx - x;
  // int py = (c->y *scale) + offy - y;

  // if (m) {
  //   int tmp[6];
  //   matrix_inverse(m,tmp);
  //   int xx = px;
  //   int yy = py;

  //   px = (xx * tmp[0] + yy * tmp[2]) / 1024 + tmp[4];
  //   py = (xx * tmp[1] + yy * tmp[3]) / 1024 + tmp[5];
  // }

  // return rect_in(rect->screen,px,py,m);
  return rect_in(rect->screen,x,y,m,c,offx,offy,scale,mirror,extend);
}

int
sprite_test(struct ej_package * pack, struct picture * pic, int x, int y, int *mat, struct ej_cursor *c, int offx, int offy, float scale, int mirror, int extend) {
  assert(pic->n < 0);
  int i;
  for (i=0;i<-pic->n;i++) {
    int r = test(pack, x,y, mat, &pic->part[i], c, offx, offy, scale, mirror, extend);
    if (r) {
      return r;
    }
  }
  return 0;
}

void 
sprite_region(struct ej_package* pack, struct picture* pic, int x, int y, int *mat, int offx, int offy, float scale, int mirror, struct ej_rect* rect)
{
  assert(pic->n < 0);
  int i;
  for (i=0;i<-pic->n;i++) {
    _get_region(pic->part[i].screen,x,y,mat,offx,offy,scale,mirror,rect);
  }
}