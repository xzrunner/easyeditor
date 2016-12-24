#include "gl/RenderTarget.h"

#include <ejoy2d/opengl.h>
#include <ejoy2d/render.h>

#include <logger.h>

namespace ur
{
namespace gl
{

RenderTarget::RenderTarget()
{
	m_curr = render_query_target();
}

int RenderTarget::Create(int id)
{
	GLuint gl_id = id;
	glGenFramebuffers(1, &gl_id);
	return gl_id;
}

void RenderTarget::Release(int id)
{
	GLuint gl_id = id;
	glDeleteFramebuffers(1, &gl_id);
}

void RenderTarget::BindTarget(int id)
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void RenderTarget::BindTexture(int tex_id)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0);
}

int RenderTarget::CheckStatus()
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		return 1;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		LOGW("%s", "Framebuffer incomplete: Attachment is NOT complete.\n");
		return 0;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		LOGW("%s", "Framebuffer incomplete: No image is attached to FBO.\n");
		return 0;
#if !defined(_WIN32) && !defined(__MACOSX)
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
		LOGW("%s", "Framebuffer incomplete: Attached images have different dimensions.\n");
		return 0;
#endif
	case GL_FRAMEBUFFER_UNSUPPORTED:
		LOGW("%s", "Unsupported by FBO implementation.\n");
		return 0;
	default:
		LOGW("%s", "Unknow error.\n");
		return 0;
	}
}

}
}