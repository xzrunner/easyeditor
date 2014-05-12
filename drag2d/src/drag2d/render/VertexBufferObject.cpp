/**
* Copyright (c) 2006-2012 LOVE Development Team
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
**/

#include "VertexBufferObject.h"

#include "../common/Exception.h"

#include <stdlib.h>

#include <gl/GLee.h>

using namespace d2d;

VertexBufferObject::VertexBufferObject(size_t size, int target, int usage)
	: VertexBuffer(size, target, usage)
	, m_vbo(0)
	, m_mapped(NULL)
	, m_bufferCopy(NULL)
{
// 	std::string strVersion((char *)glGetString(GL_VERSION));
// 	if ((int)strVersion.find("1.0") != -1)
//  		throw d2d::Exception("Not supported");

	bool ok = load(false);

	if (!ok)
		throw d2d::Exception("Could not load VBO.");
}

VertexBufferObject::~VertexBufferObject()
{
	if (m_vbo)
		unload(false);
}

void* VertexBufferObject::map()
{
	if (m_mapped)
		throw d2d::Exception("VBO is already mapped!");

	m_mapped = malloc(getSize());
	if (!m_mapped)
		throw d2d::Exception("Out of memory!");
//	glGetBufferSubData(getTarget(), 0, getSize(), m_mapped);

	return m_mapped;
}

void VertexBufferObject::unmap()
{
	glBufferSubData(getTarget(), 0, getSize(), m_mapped);
	free(m_mapped), m_mapped = NULL;
}

void VertexBufferObject::bind()
{
	glBindBuffer(getTarget(), m_vbo);
}

void VertexBufferObject::unbind()
{
	glBindBuffer(getTarget(), 0);
}

void VertexBufferObject::fill(size_t offset, size_t size, const void* data)
{
 	if (m_mapped)
 		memcpy(static_cast<char*>(m_mapped) + offset, data, size);
 	else
		glBufferSubData(getTarget(), offset, size, data);
}

const void* VertexBufferObject::getPointer(size_t offset) const
{
	return reinterpret_cast<const void*>(offset);	
}

bool VertexBufferObject::loadVolatile()
{
	return load(true);
}

void VertexBufferObject::unloadVolatile()
{
	unload(true);
}

bool VertexBufferObject::load(bool restore)
{
	GLuint vbo = m_vbo;
	glGenBuffers(1, &vbo);

	bind();

	// Copy the old buffer only if 'restore' was requested.
	const GLvoid *src = restore ? m_bufferCopy : 0;

//	while (GL_NO_ERROR != glGetError())
		/* clear error messages */;

	// Note that if 'src' is '0', no data will be copied.
	glBufferData(getTarget(), getSize(), src, getUsage());
	GLenum err = glGetError();

	delete[] m_bufferCopy, m_bufferCopy = NULL;

	unbind();

	return (GL_NO_ERROR == err);
}

void VertexBufferObject::unload(bool save)
{
	delete[] m_bufferCopy, m_bufferCopy = NULL;

	// Save data before unloading.
	if (save)
	{
		bind();

		GLint size;
		glGetBufferParameteriv(getTarget(), GL_BUFFER_SIZE, &size);

		const byte* src = static_cast<byte*>(map());
		if (src)
		{
			m_bufferCopy = new byte[size];
			memcpy(m_bufferCopy, src, size);
			unmap();
		}

		unbind();
	}

	GLuint vbo = m_vbo;
	glDeleteBuffers(1, &vbo);
	m_vbo = 0;
}
