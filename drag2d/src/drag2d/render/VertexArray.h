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

#ifndef D2D_VERTEX_ARRAY_H
#define D2D_VERTEX_ARRAY_H

#include "VertexBuffer.h"

#include <stdint.h>

namespace d2d
{
	class VertexArray : public VertexBuffer
	{
	public:
		VertexArray(size_t size, int target, int usage);
		virtual ~VertexArray();
		
		virtual void* map();
		virtual void unmap();

		virtual void bind();
		virtual void unbind();

		virtual void fill(size_t offset, size_t size, const void* data);

		virtual const void* getPointer(size_t offset) const ;

	private:
		uint8_t* m_data;

	}; // VertexArray
}

#endif // D2D_VERTEX_ARRAY_H
