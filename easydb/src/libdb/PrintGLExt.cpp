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

	const char* extensions = (char*)glGetString(GL_EXTENSIONS);
	printf("%s\n", extensions);

	return 0;
}

}