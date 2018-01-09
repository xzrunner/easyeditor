#include "PrintGLExt.h"
#include "utility.h"

#include <gl/glew.h>

namespace edb
{

std::string PrintGLExt::Command() const
{
	return "gl-ext";
}

std::string PrintGLExt::Description() const
{
	return "print opengl extension";
}

std::string PrintGLExt::Usage() const
{
	return Command();
}

int PrintGLExt::Run(int argc, char *argv[])
{
	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	GLint n;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);
	for (GLint i = 0; i < n; ++i) {
		printf("%s\n", (const char*)(glGetStringi(GL_EXTENSIONS, i)));
	}

	return 0;
}

}