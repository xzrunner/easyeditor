#pragma once

#include "VertexBuffer.h"

namespace d2d
{
	class ImageSprite;
	class Image;

	class SpriteBatch
	{
	public:
		enum UsageHint
		{
			USAGE_DYNAMIC = 1,
			USAGE_STATIC,
			USAGE_STREAM,
			USAGE_MAX_ENUM
		};

	public:
		SpriteBatch(int size, int usage);
		virtual ~SpriteBatch();

		int add(const ImageSprite* sprite, int index = -1);

		void* lock();
		void unlock();

		void OnDraw() const;

		int getAndResetDrawTimes() {
			int times = m_drawTimes;
			m_drawTimes = 0;
			return times;
		}

		void clear() {
			m_next = 0;
			m_drawTimes = 0;
		}

	private:
		void createBuffer(int size, int usage);
		void fillIndicesBuffer(int size);

		void add(const Vertex* v, int index);

	private:
		static const int X_MIRROR = 0x0001;
		static const int Y_MIRROR = 0x0002;

	private:
		unsigned int m_textureID;

		int m_size;
		int m_next;

		mutable int m_drawTimes;

		float m_node[6];
		Vertex m_sprite[4];

		VertexBuffer* m_vertices;
		VertexBuffer* m_indices;

//		static void draw(const std::vector<Sprite*>& sprites) const;

	}; // SpriteBatch
}