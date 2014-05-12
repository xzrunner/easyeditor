#pragma once

#include <set>

namespace d2d
{
	class RawPixels
	{
	public:
		RawPixels();
		~RawPixels();

 		void loadFromFile(const char* filepath);
// 		unsigned int storeToTexture(unsigned int old) const;

		const unsigned char* getPixelData() const;
		void getSize(int& width, int& height) const;
		int getChannels() const;

	public:
		struct PixelsPos
		{
			int x, y;
		}; // PixelsPos

		class PixelCmp
		{
		public:
			bool operator () (const PixelsPos* lhs, const PixelsPos* rhs) const {
				return lhs->x < rhs->x
					|| lhs->x == rhs->x && lhs->y < rhs->y;
			}
		}; // PixelCmp

		typedef std::set<PixelsPos*, PixelCmp> PixelBuf;

		void replaceBySelected(const PixelBuf& selected);

	private:
		unsigned char* m_pixelData;
		int m_width, m_height, m_channels;

	}; // RawPixels
}

