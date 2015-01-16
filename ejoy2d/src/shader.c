#include "shader.h"
#include "fault.h"
#include "dtex_facade.h"

#include "opengl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "platform.h"

// #define DRAWCALL_STAT

#define MAX_COMMBINE 1024
// #define MAX_COMMBINE 1
#define ATTRIB_VERTEX 0
#define ATTRIB_TEXTCOORD 1
#define ATTRIB_COLOR 2
#define ATTRIB_ADDITIVE 3

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//#define TEST_LOG

#define DTEX_WIDTH 2048
#define DTEX_HEIGHT 2048

#ifdef USE_DTEX
struct dtex* Dtex = NULL;
#endif

static GLuint VertexBuffer = 0;
static GLuint IndexBuffer = 0;

#ifdef DRAWCALL_STAT
static int drawcall = 0;
#endif

struct program {
  GLuint prog;
  GLint sampler0, sampler1;
};

static struct program Prog[7];
#define PROGRAM_TOTAL (sizeof(Prog)/sizeof(Prog[0]))

struct render_state {
  int edge;
  unsigned int color;
  unsigned int additive;
  int blend;
  int activeprog;
  int tex;
  int fbo;
  int object;
  float vb[24 * MAX_COMMBINE];
};

static struct render_state *RS = NULL;

static void
rs_init() {
  struct render_state * rs = malloc(sizeof(*rs));
  rs->edge = 0;
  rs->color = 0xffffffff;
  rs->additive = 0;
  rs->blend = GL_ONE_MINUS_SRC_ALPHA;
  rs->activeprog = -1;
  rs->tex = 0;
  rs->fbo = 0;
  rs->object = 0;

  glBlendFunc(GL_ONE, rs->blend);

  glGenBuffers(1, &IndexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

  size_t size = MAX_COMMBINE * 6 * sizeof(GLushort);
  GLushort* idxs = malloc(size);
  int i;
  for (i=0;i<MAX_COMMBINE;i++) {
    idxs[i*6] = i*4;
    idxs[i*6+1] = i*4+1;
    idxs[i*6+2] = i*4+2;
    idxs[i*6+3] = i*4;
    idxs[i*6+4] = i*4+2;
    idxs[i*6+5] = i*4+3;
  }
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, idxs, GL_STATIC_DRAW);
  free(idxs);

  RS = rs;
}

static void
rs_commit() {
  if (RS == NULL || RS->object == 0)
    return;

  glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, 24 * RS->object * sizeof(float), RS->vb, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(ATTRIB_VERTEX);
  glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(ATTRIB_TEXTCOORD);
  glVertexAttribPointer(ATTRIB_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(8));
  glEnableVertexAttribArray(ATTRIB_COLOR);
  glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_FALSE, 24, BUFFER_OFFSET(16));
  glEnableVertexAttribArray(ATTRIB_ADDITIVE);
  glVertexAttribPointer(ATTRIB_ADDITIVE, 4, GL_UNSIGNED_BYTE, GL_FALSE, 24, BUFFER_OFFSET(20));  
  glDrawElements(GL_TRIANGLES, 6 * RS->object, GL_UNSIGNED_SHORT, 0);
    
  RS->object = 0;
#ifdef DRAWCALL_STAT
  drawcall++;
#endif
}

static GLuint
compile(const char * source, int type) {
  GLint status;

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (status == 0) {
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

static void
link(struct program *p) {
  GLint status;
  glLinkProgram(p->prog);

  glGetProgramiv(p->prog, GL_LINK_STATUS, &status);
  if (status == 0) {
    fault("Can't link program");
  }
}

static void
program_init(struct program * p, const char *FS, const char *VS) {
  // Create shader program.
  p->prog = glCreateProgram();

  GLuint fs = compile(FS, GL_FRAGMENT_SHADER);
  if (fs == 0) {
    fault("Can't compile fragment shader");
  } else {
    glAttachShader(p->prog, fs);
  }

  GLuint vs = compile(VS, GL_VERTEX_SHADER);
  if (vs == 0) {
    fault("Can't compile vertex shader");
  } else {
    glAttachShader(p->prog, vs);
  }

  glBindAttribLocation(p->prog, ATTRIB_VERTEX, "position");
  glBindAttribLocation(p->prog, ATTRIB_TEXTCOORD, "texcoord");
  glBindAttribLocation(p->prog, ATTRIB_COLOR, "color");
  glBindAttribLocation(p->prog, ATTRIB_ADDITIVE, "additive");

  link(p);

  p->sampler0 = glGetUniformLocation(p->prog, "texture0");
  if (p == &Prog[PROGRAM_SPRITE_PKM] || p == &Prog[PROGRAM_GRAY_PKM])
    p->sampler1 = glGetUniformLocation(p->prog, "texture1");
  else
    p->sampler1 = 0;

  glDetachShader(p->prog, fs);
  glDeleteShader(fs);
  glDetachShader(p->prog, vs);
  glDeleteShader(vs);
}

void
shader_load() {
  if (RS)
    return;

#if 0
#define FLOAT_PRECISION \
  "#ifdef GL_FRAGMENT_PRECISION_HIGH  \n" \
  "precision highp float;  \n" \
  "#else  \n" \
  "precision lowp float;  \n" \
  "#endif  \n"
#else
#define FLOAT_PRECISION \
  "precision lowp float;  \n"
#endif

  static const char * sprite_fs =
  FLOAT_PRECISION

  "varying vec4 v_fragmentColor; \n"
  "varying vec4 v_fragmentAddi; \n"
  "varying vec2 v_texcoord;  \n"
  "uniform sampler2D texture0;  \n"
  "\n"
  "void main()  \n"
  "{  \n"
  "  gl_FragColor = texture2D(texture0, v_texcoord);  \n"  
  // "  gl_FragColor = vec4(1, 1, 1, 1);  \n"  

  "  vec4 tmp = texture2D(texture0, v_texcoord);  \n"
  "  if (tmp.a == 0.0) \n"
  "    discard;        \n"
  "  gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz;  \n"
  "  gl_FragColor.w = tmp.w;    \n"
  "  gl_FragColor *= v_fragmentColor.w;  \n"
  "  gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w;  \n"
  "}  \n"
  ;

  static const char* sprite_ktx_fs =
  "precision mediump float;  \n"

  "varying vec4 v_fragmentColor; \n"
  "varying vec4 v_fragmentAddi; \n"
  "varying vec2 v_texcoord;  \n"
  "uniform sampler2D texture0;  \n"
  "\n"
  "void main()  \n"
  "{  \n"  
  "  vec4 tmp = texture2D(texture0, v_texcoord);  \n"
  "  gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz;  \n"
  "  gl_FragColor.w = texture2D(texture0, vec2(v_texcoord.x, v_texcoord.y + 0.5)).r;    \n"
  "  gl_FragColor *= v_fragmentColor.w;  \n"
  "  gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w;  \n"
  "}  \n"
  ;

  static const char * sprite_pkm_fs =
  FLOAT_PRECISION

  "varying vec4 v_fragmentColor; \n"
  "varying vec4 v_fragmentAddi; \n"
  "varying vec2 v_texcoord;  \n"
  "uniform sampler2D texture0;  \n"
  "uniform sampler2D texture1;  \n"
  "\n"
  "void main()  \n"
  "{  \n"  
  "  vec4 tmp = texture2D(texture0, v_texcoord);  \n"
  "  tmp.w = texture2D(texture1, v_texcoord).r;  \n"
  "  gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz;  \n"
  "  gl_FragColor.w = tmp.w;    \n"
  "  gl_FragColor *= v_fragmentColor.w;  \n"
  "  gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w;  \n"
  "}  \n"
  ;

  static const char * sprite_vs =
  FLOAT_PRECISION
  "\n"
  "attribute vec4 position;  \n"
  "attribute vec2 texcoord;  \n"
  "attribute vec4 color;     \n"
  "attribute vec4 additive;     \n"
  "\n"
  "varying vec2 v_texcoord;  \n"
  "varying vec4 v_fragmentColor;  \n"
  "varying vec4 v_fragmentAddi; \n"
  "\n"
  "void main()  \n"
  "{  \n"
  "  gl_Position = position;  \n"
  "  v_fragmentColor = color / 255.0; \n"
  "  v_fragmentAddi = additive / 255.0; \n"
  "  v_texcoord = texcoord;  \n"
  "}  \n"
  ;

  static const char * gray_fs =
  FLOAT_PRECISION

  "varying vec4 v_fragmentColor; \n"  
  "varying vec4 v_fragmentAddi; \n"
  "varying vec2 v_texcoord;  \n"
  "uniform sampler2D texture0;  \n"
  "\n"
  "void main()  \n"
  "{  \n"
  "  vec4 tmp = texture2D(texture0, v_texcoord);  \n"
  "  vec4 c;  \n"
  "  c.xyz = tmp.xyz * v_fragmentColor.xyz;  \n"
  "  c.w = tmp.w;    \n"
  "  c *= v_fragmentColor.w;  \n"
  "  c.xyz += v_fragmentAddi.xyz * tmp.w;  \n"
  "  float g = dot(c.rgb , vec3(0.299, 0.587, 0.114));  \n"
  "  gl_FragColor = vec4(g,g,g,c.a);  \n"
  "}  \n"
  ;

  static const char * gray_ktx_fs =
  FLOAT_PRECISION

  "varying vec4 v_fragmentColor; \n"  
  "varying vec4 v_fragmentAddi; \n"
  "varying vec2 v_texcoord;  \n"
  "uniform sampler2D texture0;  \n"
  "\n"
  "void main()  \n"
  "{  \n"
  "  vec4 tmp = texture2D(texture0, v_texcoord);  \n"
  "  vec4 c;  \n"
  "  c.xyz = tmp.xyz * v_fragmentColor.xyz;  \n"
  "  c.w = texture2D(texture0, vec2(v_texcoord.x, v_texcoord.y + 0.5)).r;    \n"
  "  c *= v_fragmentColor.w;  \n"
  "  c.xyz += v_fragmentAddi.xyz * tmp.w;  \n"
  "  float g = dot(c.rgb , vec3(0.299, 0.587, 0.114));  \n"
  "  gl_FragColor = vec4(g,g,g,c.a);  \n"
  "}  \n"
  ;

  static const char * gray_pkm_fs =
  FLOAT_PRECISION

  "varying vec4 v_fragmentColor; \n"  
  "varying vec4 v_fragmentAddi; \n"
  "varying vec2 v_texcoord;  \n"
  "uniform sampler2D texture0;  \n"
  "uniform sampler2D texture1;  \n"
  "\n"
  "void main()  \n"
  "{  \n"
  "  vec4 tmp = texture2D(texture0, v_texcoord);  \n"
  "  tmp.w = texture2D(texture1, v_texcoord).r;  \n"
  "  vec4 c;  \n"
  "  c.xyz = tmp.xyz * v_fragmentColor.xyz;  \n"
  "  c.w = tmp.w;    \n"
  "  c *= v_fragmentColor.w;  \n"
  "  c.xyz += v_fragmentAddi.xyz * tmp.w;  \n"
  "  float g = dot(c.rgb , vec3(0.299, 0.587, 0.114));  \n"
  "  gl_FragColor = vec4(g,g,g,c.a);  \n"
  "}  \n"
  ;

  rs_init();

  program_init(&Prog[PROGRAM_SPRITE], sprite_fs, sprite_vs);
  program_init(&Prog[PROGRAM_GRAY], gray_fs, sprite_vs);
  program_init(&Prog[PROGRAM_SPRITE_KTX], sprite_ktx_fs, sprite_vs);
  program_init(&Prog[PROGRAM_GRAY_KTX], gray_ktx_fs, sprite_vs);
  program_init(&Prog[PROGRAM_SPRITE_PKM], sprite_pkm_fs, sprite_vs);
  program_init(&Prog[PROGRAM_GRAY_PKM], gray_pkm_fs, sprite_vs);

  shader_program(PROGRAM_SPRITE);

  glGenBuffers(1, &VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

  glEnable(GL_BLEND);

#ifdef USE_DTEX
  assert(Dtex == NULL);
  Dtex = dtex_create(DTEX_WIDTH, DTEX_HEIGHT);
#endif
}

void
shader_unload() {
  if (RS == NULL) {
    return;
  }
//  shader_texture(0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);        
  if (Prog[RS->activeprog].sampler1) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);     
  }

  int i;
  for (i=0;i<PROGRAM_TOTAL;i++) {
    glDeleteProgram(Prog[i].prog);
  }

  glDeleteBuffers(1,&VertexBuffer);
  glDeleteBuffers(1,&IndexBuffer);
  free(RS);
  RS = NULL;

#ifdef USE_DTEX
  dtex_release(Dtex);
  Dtex = NULL;
#endif
}

void
shader_color(unsigned int color, unsigned int addi){
  if (color == 0xffffffff) {
    RS->color = 0xffffffff;
  } else {
    unsigned char c[4];
    c[0] = ((color >> 24) & 0xff);
    c[1] = ((color >> 16) & 0xff);
    c[2] = ((color >> 8) & 0xff);
    c[3] = (color & 0xff );
    RS->color = (c[3] << 24 | c[0] << 16 | c[1] << 8 | c[2]);
  }
  if (addi == 0) {
    RS->additive = 0;
  } else {
    unsigned char c[3];
    c[0] = ((addi >> 16) & 0xff);
    c[1] = ((addi >> 8) & 0xff);
    c[2] = (addi & 0xff);
    RS->additive = (0xff << 24 | c[0] << 16 | c[1] << 8 | c[2]);
  }
}

void
shader_program(int n) {
  if (RS->activeprog != n) {
#ifdef TEST_LOG
    pf_log("shader_program");
#endif
    rs_commit();
    RS->activeprog = n;
    glUseProgram(Prog[n].prog);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    RS->tex = 0;

    if (n == PROGRAM_SPRITE_PKM || n == PROGRAM_GRAY_PKM) {
      glUniform1i(Prog[n].sampler0, 0);
      glUniform1i(Prog[n].sampler1, 1);
    } else {
      glUniform1i(Prog[n].sampler0, 0);
    }
  }
}

void
shader_texture(int id) {
  if (RS->tex != id) {
#ifdef TEST_LOG
    pf_log("RS->tex != id, id %d", id);
#endif
    rs_commit();
    RS->tex = (GLuint)id;
    glBindTexture(GL_TEXTURE_2D, RS->tex);
  }
}

void 
shader_fbo(int id) {
  if (RS->fbo != id) {
#ifdef TEST_LOG
    pf_log("RS->fbo != id, id %d", id);
#endif    
    rs_commit();
    RS->fbo = (GLuint)id;
    glBindFramebuffer(GL_FRAMEBUFFER, RS->fbo); 
  }
}

int 
shader_get_fbo() {
  return RS->fbo;
}

static void
_copy_vertex(const float vb[16]) {
  float* ptr = RS->vb + 24 * RS->object;
  memcpy(ptr, vb, 4 * sizeof(float));
  ptr += 4;
  memcpy(ptr, &RS->color, sizeof(int));
  ptr += 1;
  memcpy(ptr, &RS->additive, sizeof(int));
  ptr += 1;  
  memcpy(ptr, &vb[4], 4 * sizeof(float));
  ptr += 4;
  memcpy(ptr, &RS->color, sizeof(int));
  ptr += 1;
  memcpy(ptr, &RS->additive, sizeof(int));
  ptr += 1;    
  memcpy(ptr, &vb[8], 4 * sizeof(float));
  ptr += 4;
  memcpy(ptr, &RS->color, sizeof(int));
  ptr += 1;
  memcpy(ptr, &RS->additive, sizeof(int));
  ptr += 1;    
  memcpy(ptr, &vb[12], 4 * sizeof(float));
  ptr += 4;
  memcpy(ptr, &RS->color, sizeof(int));
  ptr += 1; 
  memcpy(ptr, &RS->additive, sizeof(int));
}

void
shader_draw(const float vb[16], int texid) {
  shader_texture(texid);

  _copy_vertex(vb);
  if (++RS->object >= MAX_COMMBINE) {
#ifdef TEST_LOG
    pf_log("RS->object >= MAX_COMMBINE");
#endif
    rs_commit();
  }
}

void 
shader_draw_separate(const float vb[16], int id_rgb, int id_alpha) {
  if (RS->tex != id_rgb) {
#ifdef TEST_LOG
    pf_log("RS->tex != id, id %d", id_rgb);
#endif
    rs_commit();
    RS->tex = (GLuint)id_rgb;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_rgb);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, id_alpha);
    glActiveTexture(GL_TEXTURE0);
  }  

  _copy_vertex(vb);
  if (++RS->object >= MAX_COMMBINE) {
#ifdef TEST_LOG
    pf_log("RS->object >= MAX_COMMBINE");
#endif
    rs_commit();
  }  
}

void
shader_blend(int mode) {
  if (mode != RS->blend) {
#ifdef TEST_LOG
    pf_log("shader_blend");
#endif
    rs_commit();
    RS->blend = mode;
    glBlendFunc(GL_ONE, mode);
  }
}

void
shader_flush() {
  // GLenum err = glGetError();
  // if (err != GL_NO_ERROR){
  //   pf_log("shader_flush glError: 0x%04X \n", err);
  //   return;
  // }  

#ifdef DRAWCALL_STAT
  static int last = 0;
  pf_log("draw times: %d \n", drawcall - last);
  last = drawcall;
#endif
      
#ifdef TEST_LOG
    pf_log("shader_flush");
#endif
//#ifdef DTEX_DEBUG
//  debug_draw(Dtex);
//  dtexf_debug_draw();
//#endif
  rs_commit();
}

void
shader_scissor(int x, int y, int width, int height, int enable) {
// #ifdef  TEST_LOG
//   pf_log("shader_scissor");
// #endif
//   rs_commit();
//   if (enable) {
//     struct ej_screen scr;
//     ejoy_get_screen(&scr);
//     float s = scr.scale;
//     glEnable(GL_SCISSOR_TEST);
//     glScissor(x*s, (scr.h-height-y)*s, width*s, height*s);
//   } else {
//     glDisable(GL_SCISSOR_TEST);
//   }
}

// void
// shader_viewport(int x, int y, int w, int h) {
//   struct ej_screen scr;
//   rs_commit();
//   ejoy_get_screen(&scr);
//   glViewport(x * scr.scale, y * scr.scale, w * scr.scale, h * scr.scale);
// }
