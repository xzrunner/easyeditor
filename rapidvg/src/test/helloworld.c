#include "rvg_render.h"
#include "rvg_shader.h"

#include <gl/glew.h>
#include <gl/freeglut.h>

void 
display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	rvg_rect(10, 10, 100, 100);

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

	glutInitWindowSize(250, 250);

	glutInitWindowPosition(100, 100);

	glutCreateWindow("hello");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);

	glutMainLoop();

	return 0;
}