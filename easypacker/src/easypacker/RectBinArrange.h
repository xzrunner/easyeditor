#ifndef EPACKER_RECT_BIN_ARRANGE_H
#define EPACKER_RECT_BIN_ARRANGE_H

#include "BaseStrategy.h"

#include "Rect.h"

namespace epacker
{
	class RectBinArrange : public BaseStrategy
	{
	public:
		virtual void arrange(const std::vector<d2d::ImageSprite*>& sprites);
		virtual int GetTextureAccount() const {
			return m_tex_account;
		}

	private:
		void BeforePacking(const std::vector<d2d::ImageSprite*>& sorted, 
			std::vector<RectSize>& input) const;
		void AfterPacking(float xoffset,
			std::vector<d2d::ImageSprite*>& sprites, 
			const std::vector<Rect>& output, 
			std::vector<d2d::ImageSprite*>& remains) const;

		void GuillotineBinPackAlg(const std::vector<RectSize>& input, std::vector<Rect>& output) const;
		void MaxRectsBinPackAlg(std::vector<RectSize>& input, std::vector<Rect>& output) const;
		void ShelfBinPackAlg(const std::vector<RectSize>& input, std::vector<Rect>& output) const;
		void SkylineBinPackAlg(std::vector<RectSize>& input, std::vector<Rect>& output) const;

	private:
		int m_tex_account;

	}; // RectBinArrange
}

#endif // EPACKER_RECT_BIN_ARRANGE_H