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

#include "VertexArray.h"

#include <string>

using namespace d2d;

VertexArray::VertexArray(size_t size, int target, int usage)
	: VertexBuffer(size, target, usage)
{
	m_data = new uint8_t[size];
}

VertexArray::~VertexArray()
{
	delete[] m_data;
}

void* VertexArray::map()
{
	return m_data;
}

void VertexArray::unmap()
{
}

void VertexArray::bind()
{
}

void VertexArray::unbind()
{
}

void VertexArray::fill(size_t offset, size_t size, const void* data)
{
	memcpy(m_data + offset, data, size);
}

const void* VertexArray::getPointer(size_t offset) const
{
	return m_data + offset;
}
