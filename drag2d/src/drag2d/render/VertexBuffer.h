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

#ifndef D2D_VERTEX_BUFFER_H
#define D2D_VERTEX_BUFFER_H

namespace d2d
{
	struct Vertex
	{
		unsigned char r, g, b, a;
		float x, y;
		float u, v;

		Vertex() : r(255), g(255), b(255), a(255) {}
	};

	class VertexBuffer
	{
	public:

		static VertexBuffer* create(size_t size, int target, int usage);

		virtual ~VertexBuffer() {}
	
		virtual void* map() = 0;
		virtual void unmap() = 0;

		virtual void bind() = 0; 
		virtual void unbind() = 0;

		virtual void fill(size_t offset, size_t size, const void* data) = 0;

		virtual const void* getPointer(size_t offset) const = 0;

		size_t getSize() const { return m_size; }
		int getTarget() const { return m_target; }
		int getUsage() const { return m_usage; }

	protected:
		VertexBuffer(size_t size, int target, int usage);

	private:
		size_t m_size;

		int m_target;

		int m_usage;

	}; // VertexBuffer
}

#endif // D2D_VERTEX_BUFFER_H
