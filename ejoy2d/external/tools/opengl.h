#ifndef ejoy2d_opengl_h
#define ejoy2d_opengl_h

#ifdef __APPLE__

#include <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES1/glext.h>

#elif defined(linux) || defined(__linux) || defined(__linux__)

//#define USE_LOD

#include <GLES2/gl2.h> 
#include <GLES2/gl2ext.h>

#else

#include <GL/glew.h>

#endif

#endif
