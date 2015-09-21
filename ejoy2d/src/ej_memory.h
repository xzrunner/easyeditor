#ifndef __EJ_MEMORY_H__
#define __EJ_MEMORY_H__

typedef struct ej_memory_t ej_memory_t;

ej_memory_t *ejoy_memory_create();
void ejoy_memory_free(ej_memory_t *m);

void ejoy_memory_report(ej_memory_t *m);

void *ejoy_malloc(ej_memory_t *m, size_t nsize);
void ejoy_free(ej_memory_t *m, void *ptr, size_t osize);
void *ejoy_realloc(ej_memory_t *m, void *ptr, size_t osize, size_t nsize);

#endif
