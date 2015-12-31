#include "rvg_render.h"
#include "rvg_shader.h"

#include <gl/glew.h>
#include <gl/freeglut.h>

void 
display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	rvg_shader_color(0xff0000ff);
	rvg_point(0, 0, 5);
	rvg_shader_color(0xffff00ff);
	rvg_line(-100, -100, -50, -80);
	rvg_shader_color(0xffffffff);
	rvg_rect(10, 10, 100, 100, true);

	rvg_circle(100, -100, 50, false);

	rvg_shader_color(0xff00ffff);
	rvg_set_circle_segments(32);
	rvg_circle(200, -100, 50, true);

	rvg_shader_flush();

	glFlush();
}

void 
init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);

	rvg_shader_init();
	rvg_shader_load();
	rvg_shader_bind();

	rvg_shader_modelview(0, 0, 1);
}

void 
resize(int width, int height) {
	glViewport(0, 0, width, height);
	rvg_shader_projection(width, height);
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