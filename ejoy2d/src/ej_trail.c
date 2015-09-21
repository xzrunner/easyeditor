#include "sprite.h"
#include "spritepack.h"
#include "shader.h"
#include "screen.h"
#include "matrix.h"
#include "ej_trail.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static float
fast_inv_sqrt_f( float number ) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//    y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}

static inline void
normalize(float *x, float *y) {
    float inv_dist = fast_inv_sqrt_f((*x)*(*x) + (*y)*(*y));
    (*x) *= inv_dist;
    (*y) *= inv_dist;
}

void
drawtrail(struct sprite *s, struct ej_trail_t *trail, const struct srt *srt, const struct sprite_trans *arg) {
    int i, j;
    struct vertex_pack vb[4];
    struct matrix tmp;
    if (arg->mat == NULL) {
        matrix_identity(&tmp);
    } else {
        tmp = *arg->mat;
    }
    int *m = tmp.m;

    float k;
    int nn = trail->seg_count;
    float extend = trail->extend;

    struct ej_trail_point_t *pts = trail->pts;
    // init
    if(!trail->inited) {
        for(i=0; i<nn; ++i) {
            pts[i].x = m[4];
            pts[i].y = m[5];
        }
        trail->inited = true;
    }
    else {
        // update points
        if(++trail->update_counter >= trail->update_rate) {
            trail->update_counter = 0;
            for(i=0; i<nn-1; ++i) {
                struct ej_trail_point_t *f = &pts[i];
                struct ej_trail_point_t *t = &pts[i+1];
                f->x = t->x;
                f->y = t->y;
            }
        }
        pts[nn-1].x = m[4];
        pts[nn-1].y = m[5];
    }

    float xx[TRAIL_MAX_SEG_COUNT*2];
    float yy[TRAIL_MAX_SEG_COUNT*2];
    uint16_t alpha[TRAIL_MAX_SEG_COUNT];

    struct ej_trail_point_t *pprev = NULL;
    struct ej_trail_point_t *pcur  = NULL;
    struct ej_trail_point_t *pnext = NULL;

    float dx, dy;
    uint16_t afac = (uint16_t)((trail->color >> 24) * 0xffff / 0xff);

    // tail
    pprev = NULL;
    pcur  = &pts[0];
    pnext = &pts[1];

    dx = pnext->x - pcur->x;
    dy = pnext->y - pcur->y;
    normalize(&dx, &dy);

    k = 0.1f;
    xx[0] = pcur->x + (-dy) * extend * k;
    yy[0] = pcur->y + ( dx) * extend * k;
    xx[1] = pcur->x + ( dy) * extend * k;
    yy[1] = pcur->y + (-dx) * extend * k;
    alpha[0] = (uint16_t)(afac*0.1f);

    // middle
    for(i=1;i<nn-1;++i) {
        pprev = &pts[i-1];
        pcur  = &pts[i];
        pnext = &pts[i+1];

        float dx1 = pcur->x - pprev->x;
        float dy1 = pcur->y - pprev->y;
        float dx2 = pnext->x - pcur->x;
        float dy2 = pnext->y - pcur->y;
        dx = (dx1 + dx2) * 0.5f;
        dy = (dy1 + dy2) * 0.5f;
        normalize(&dx, &dy);

        k = 0.1f + (i * 0.9f)/(nn-2);
        xx[i*2]   = pcur->x + (-dy) * extend * k;
        yy[i*2]   = pcur->y + ( dx) * extend * k;
        xx[i*2+1] = pcur->x + ( dy) * extend * k;
        yy[i*2+1] = pcur->y + (-dx) * extend * k;
        alpha[i] = (uint16_t)(afac*k);
    }

    // head
    pprev = &pts[nn-2];
    pcur  = &pts[nn-1];
    pnext = NULL;

    dx = pcur->x - pprev->x;
    dy = pcur->y - pprev->y;
    normalize(&dx, &dy);

    xx[(nn-1)*2]   = pcur->x + (-dy) * extend;
    yy[(nn-1)*2]   = pcur->y + ( dx) * extend;
    xx[(nn-1)*2+1] = pcur->x + ( dy) * extend;
    yy[(nn-1)*2+1] = pcur->y + (-dx) * extend;
    alpha[nn-1] = (uint16_t)(afac);

    // vertex

    matrix_identity(&tmp);
    matrix_srt(&tmp, srt);

    for(j=nn-1; j>0; --j) {
        int off[4] = { 0, 1, -1, -2 };
        int off2[4] = { 0, 0, -1, -1 };
        for(i=0; i<4; ++i) {
            float vx = xx[j*2+off[i]];
            float vy = yy[j*2+off[i]];
            vx = (vx * m[0] + vy * m[2]) / 1024 + m[4];
            vy = (vx * m[1] + vy * m[3]) / 1024 + m[5];
            screen_trans(&vx,&vy);
            vb[i].vx = vx;
            vb[i].vy = vy;
            vb[i].tx = alpha[j+off2[i]];
            vb[i].ty = 0;
        }
        shader_draw(vb, trail->color, 0, 0, 0, 0);
    }
}

