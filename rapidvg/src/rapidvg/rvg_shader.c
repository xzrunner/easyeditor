#include "rvg_shader.h"
#include "rvg_matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <gl/glew.h>

#define STRINGIFY(A)  #A
#include "normal_shader.vert"
#include "normal_shader.frag"

#define ATTRIB_VERTEX	10
#define ATTRIB_COLOR	11

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define MAX_VERTEX		1024
#define VERTEX_SIZE		3

struct shader_state {
	GLuint prog;

	GLuint modelview, projection;
	struct rvg_matrix modelview_mat, projection_mat;
	bool modelview_dirty, projection_dirty;

	GLuint vertex_buffer;

	GLenum type;

	int color;

	int count;
	float* vb;
};

static struct shader_state SS;

static GLuint
_complie(const char* source, GLuint type) {
	GLint status;

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		char buf[1024];
		glGetShaderInfoLog(shader, 1024, NULL, buf);
		glDeleteShader(shader);
		printf("compile failed:%s\n, source:\n %s\n", buf, source);
		return 0;
	}

	return shader;	
}

static void
_init_shader(const char* VS, const char* FS) {
	GLuint prog = glCreateProgram();

	GLuint vs = _complie(VS, GL_VERTEX_SHADER);
	if (vs == 0) {
		return;
	} else {
		glAttachShader(prog, vs);
	}

	GLuint fs = _complie(FS, GL_FRAGMENT_SHADER);
	if (fs == 0) {
		return;
	} else {
		glAttachShader(prog, fs);
	}

	glBindAttribLocation(prog, ATTRIB_VERTEX, "position");
	glBindAttribLocation(prog, ATTRIB_COLOR, "color");

	// link
	GLint status;
	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		char buf[1024];
		glGetProgramInfoLog(prog, 1024, NULL, buf);
		printf("link failed:%s\n", buf);
		return;
	}

	// detatch
	glDetachShader(prog, fs);
	glDeleteShader(fs);
	glDetachShader(prog, vs);
	glDeleteShader(vs);

	SS.prog = prog;

	SS.projection = glGetUniformLocation(prog, "u_projection");
	SS.modelview = glGetUniformLocation(prog, "u_modelview");
}

void 
rvg_shader_init() {
	SS.prog = 0;

	SS.modelview = SS.projection = 0;
	rvg_matrix_identity(&SS.projection_mat);
	rvg_matrix_identity(&SS.modelview_mat);
	SS.modelview_dirty = SS.projection_dirty = false;

	SS.vertex_buffer = 0;

	SS.type = 0;

	SS.color = 0xffffffff;

	SS.count = 0;
	SS.vb = NULL;
}

void 
rvg_shader_load() {
	_init_shader(normal_vert, normal_frag);

	glGenBuffers(1, &SS.vertex_buffer);

	SS.vb = (float*)malloc(sizeof(float) * VERTEX_SIZE * MAX_VERTEX);
}

void 
rvg_shader_unload() {
	glDeleteBuffers(1, &SS.vertex_buffer);

	free(SS.vb); SS.vb = NULL;
	SS.count = 0;
}

void 
rvg_shader_bind() {
	glUseProgram(SS.prog);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, SS.vertex_buffer);

	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, 12, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_FALSE, 12, BUFFER_OFFSET(8));
}

void 
rvg_shader_unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(ATTRIB_VERTEX);
	glDisableVertexAttribArray(ATTRIB_COLOR);
}

void 
rvg_shader_projection(int width, int height) {
	float hw = width * 0.5f;
	float hh = height * 0.5f;
	rvg_matrix_ortho(&SS.projection_mat, -hw, hw, -hh, hh, 1, -1);
	SS.projection_dirty = true;
}

void 
rvg_shader_modelview(float dx, float dy, float scale) {
	rvg_matrix_set_scale(&SS.modelview_mat, scale);
	rvg_matrix_translate(&SS.modelview_mat, dx, dy);
	SS.modelview_dirty = true;
}

static void 
_commit() {
	if (SS.count == 0) {
		return;
	}

	glPointSize(10);


	if (SS.projection_dirty) {
		glUniformMatrix4fv(SS.projection, 1, 0, SS.projection_mat.e);
		SS.projection_dirty = false;
	}
	if (SS.modelview_dirty) {
		glUniformMatrix4fv(SS.modelview, 1, 0, SS.modelview_mat.e);
		SS.modelview = false;
	}

	glBufferData(GL_ARRAY_BUFFER, SS.count * VERTEX_SIZE * sizeof(float), &SS.vb[0], GL_DYNAMIC_DRAW);

	glDrawArrays(SS.type, 0, SS.count);

	SS.count = 0;
}

void 
rvg_shader_color(int color) {
	SS.color = color;
}

void 
rvg_shader_type(int type) {
	if (SS.type != type) {
		_commit();
		SS.type = type;
	}
}

void 
rvg_shader_add_vertices(float* coords, int count) {
	if (SS.count + count > MAX_VERTEX) {
		_commit();
	}

	float* ptr = SS.vb + VERTEX_SIZE * SS.count;
	for (int i = 0; i < count; ++i) {
		memcpy(ptr, &coords[i*2], 2 * sizeof(float));
		ptr += 2;
		memcpy(ptr, &SS.color, sizeof(int));
		ptr += 1;
	}
	SS.count += count;
}

void 
rvg_shader_flush() {
	_commit();
}