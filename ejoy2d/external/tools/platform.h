//
//  platform.h
//  coco
//
//  Created by zixunLv on 13-12-9.
//  Copyright (c) 2013年 Cloud. All rights reserved.
//

#ifndef coco_platform_h
#define coco_platform_h

#include <stddef.h>

struct FileHandle;

struct FileHandle* pf_fileopen(const char *path, const char* format);
struct FileHandle* pf_bundleopen(const char* filename, const char* format);

size_t pf_filesize(struct FileHandle* h);
int pf_fileread(struct FileHandle* h, void *buffer, size_t size);
int pf_filewrite(struct FileHandle* h, void *buffer, size_t size);
size_t pf_fread(void *ptr, size_t size, size_t nmemb, struct FileHandle *h);
void pf_fileseek_from_cur(struct FileHandle* h, int offset);
void pf_fileseek_from_head(struct FileHandle* h, int offset);
void pf_fileclose(struct FileHandle* h);
int pf_feof(struct FileHandle* h);

#define MAX(a, b) ( ((a)>(b))?(a):(b) )
#define MIN(a, b) ( ((a)<(b))?(a):(b) )


#ifdef __ANDROID__
    #include <android/log.h>
    #include <jni.h>
    #define  LOG_TAG                    "=== ejoy ==="
    #define  pf_log(...)                __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
    #define  pf_vprint(format, ap)      __android_log_vprint(ANDROID_LOG_DEBUG, LOG_TAG, (format), (ap))
#else
    #define pf_log                      printf
    #define pf_vprint                   vprintf
#endif

#endif
