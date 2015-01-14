//
//  platform.c
//  coco
//
//  Created by zixunLv on 13-12-9.
//  Copyright (c) 2013年 Cloud. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>



#ifdef __ANDROID__
  #include <android/log.h>
  #include "android_helper.h"
  #define  LOG_TAG    "ejoy"

  struct FileHandle{
    unsigned long size;
    unsigned char* buffer;
    size_t offset;
  };

  struct FileHandle* pf_fileopen(const char *path, const char* format) {
    unsigned long sz;
    unsigned char* buffer;
    buffer = getFileData(path, format, &sz);
    if(buffer){
      struct FileHandle* ret = malloc(sizeof(*ret));
      ret->buffer = buffer;
      ret->offset = 0;
      ret->size = sz;
      return ret;
    }else
      return NULL;
  }

  size_t pf_filesize(struct FileHandle* h) {
    return h->size;
  }

  int pf_fileread(struct FileHandle* h, void *buffer, size_t size) {
    assert(h->size >= h->offset && h->size > 0 && h->buffer != NULL);
    size_t sz;
    int ret;
    if(h->offset + size <= h->size) {
      sz = size;
      ret = 1;
    } else {
      sz = h->size - h->offset;
      ret = 0;
    }
    if (sz > 0) {
      memcpy(buffer, h->buffer + h->offset, sz);
      h->offset += sz;
    } 
    return ret;
  }

  size_t pf_fread(void *ptr, size_t size, size_t nmemb, struct FileHandle *h) {
    assert(h->size >= h->offset && h->size > 0 && h->buffer != NULL);
    size_t allsize = size * nmemb;
    if(allsize == 0) {
      return 0;
    }

    if(h->offset + allsize <= h->size) {
      memcpy(ptr, h->buffer + h->offset, allsize);
      h->offset += allsize;
      return nmemb;
    }

    unsigned long rest = h->size - h->offset;
    size_t count = (size_t)(rest / size);
    if(count > 0) {
      size_t n = count * size;
      memcpy(ptr, h->buffer + h->offset, n);
      h->offset += n;
    }

    return count;
  }

  void pf_fileseek_from_cur(struct FileHandle* h, int offset) {
    h->offset += offset;
  }

  void pf_fileseek_from_head(struct FileHandle* h, int offset) {
    h->offset = offset;
  }

  void pf_fileclose(struct FileHandle* h) {
    if(h==NULL) return;

    if(h->buffer)
      free(h->buffer);
    free(h);
  }

  int pf_feof(struct FileHandle* h) {
    return (h->offset >= h->size) ? 1 : 0;
  }

  struct FileHandle* pf_bundleopen(const char* filename, const char* format){
    return pf_fileopen(filename, format);
  }

#else


#include <stdarg.h>
//   const char * BundlePath(const char * filename, const char *type, const char *directory);

  struct FileHandle{
    FILE *fp;
  };

  struct FileHandle* pf_fileopen(const char *path, const char* format) {
    FILE* _fp = fopen(path, format);
//    printf("\nopen path: %s\n", path);
    if(_fp){
      struct FileHandle* ret = malloc(sizeof(*ret));
      ret->fp = _fp;
      return ret;
    }else
      return NULL;
  }


  struct FileHandle* pf_bundleopen(const char* filename, const char* format){
//     const char* path = BundlePath(filename, "", NULL);
//     return (path)?(pf_fileopen(path, format)):(NULL);
	  return NULL;
  }

  size_t pf_filesize(struct FileHandle* h) {
    size_t save_pos = ftell(h->fp);
    fseek(h->fp, 0, SEEK_END);
    size_t sz = ftell(h->fp);
    fseek(h->fp, save_pos, SEEK_SET);
    return sz;
  }

  int pf_fileread(struct FileHandle* h, void *buffer, size_t size) {
    return fread(buffer, size, 1, h->fp);
  }

  size_t pf_fread(void *ptr, size_t size, size_t nmemb, struct FileHandle *h) {
    return fread(ptr, size, nmemb, h->fp);
  }

  void pf_fileseek_from_cur(struct FileHandle* h, int offset) {
    fseek(h->fp, offset, SEEK_CUR);
  }

  void pf_fileseek_from_head(struct FileHandle* h, int offset) {
    fseek(h->fp, offset, SEEK_SET);
  }

  void pf_fileclose(struct FileHandle* h) {
    if(h==NULL) return;

    if(h->fp)
      fclose(h->fp);
    free(h);
  }

  int pf_feof(struct FileHandle* h) {
    return feof(h->fp);
  }

  void pf_log(const char * format, ...) {
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
  }
#endif
