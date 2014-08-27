#include "DynamicUtils.h"
#include "ShaderNew.h"

#include "common/Exception.h"
#include "dataset/TPNode.h"

#include <opengl/opengl.h>
#include <gl/gl.h>
#include <gl/glu.h>

namespace d2d
{

GLuint DynamicUtils::InitTexture(int width, int height, int tex_id)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	if (tex_id == 0) {
		glGenTextures(1, (GLuint*)&tex_id);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	byte* empty_data = new byte[width*height*4];
	memset(empty_data, 0, width*height*4);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)width, (GLsizei)height, GL_RGBA, GL_UNSIGNED_BYTE, &empty_data[0]);
	delete[] empty_data;

	return tex_id;
}

GLuint DynamicUtils::InitFBO(int tex_id, int fbo_id)
{
	ShaderNew* shader = ShaderNew::Instance();

	if (fbo_id == 0) {
		glGenFramebuffersEXT(1, (GLuint*)&fbo_id);
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex_id, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		throw Exception("Create FBO error: %d", status);
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ShaderNew::Instance()->GetFboID());

	return fbo_id;
}

TPNode* DynamicUtils::InitRoot(int width, int height)
{
	TPNode* root = new TPNode(width, height);
	TPNode* c = new TPNode(width, height);
	root->SetChild(c);
	return root;
}

}