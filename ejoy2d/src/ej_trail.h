#ifndef __EJ_MEMORY_H__
#define __EJ_MEMORY_H__

#define TRAIL_DEFAULT_SEG_COUNT     (8)
#define TRAIL_MAX_SEG_COUNT         (16)
#define TRAIL_DEFAULT_UPDATE_RATE   (4)
#define TRAIL_DEFAULT_EXTEND        (32)

struct ej_trail_point_t {
    float x;
    float y;
};

struct ej_trail_t {
    uint8_t inited;
    uint8_t seg_count;
    uint8_t update_counter;
    uint8_t update_rate;
    uint32_t color;
    float extend;
    struct ej_trail_point_t pts[TRAIL_MAX_SEG_COUNT];
};

void drawtrail(struct sprite *s, struct ej_trail_t *trail, const struct srt *srt, const struct sprite_trans *arg);

#endif

