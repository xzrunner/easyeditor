#include "rvg_render.h"

#include <sl_shader.h>
#include <sl_shape.h>

#include <gl/glew.h>
#include <gl/freeglut.h>

void 
display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	sl_shape_color(0xff0000ff);
	rvg_point(0, 0, 5);
	sl_shape_color(0xffff00ff);
	rvg_line(-100, -100, -50, -80);
	sl_shape_color(0xffffffff);
	rvg_rect(10, 10, 100, 100, true);

	rvg_circle(100, -100, 50, false);

	sl_shape_color(0xff00ffff);
	rvg_set_circle_segments(32);
	rvg_circle(200, -100, 50, true);

	sl_shader_flush();

	glFlush();
}

void 
init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);

	sl_shader_init();
	sl_shape_init();

	sl_shape_modelview(0, 0, 1, 1);
}

void 
resize(int width, int height) {
	glViewport(0, 0, width, height);
	sl_shape_projection(width, height);

	sl_shape_bind();
}

int 
main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(800, 480);

	glutInitWindowPosition(100, 100);

	glutCreateWindow("hello");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);

	glutMainLoop();

	return 0;
}