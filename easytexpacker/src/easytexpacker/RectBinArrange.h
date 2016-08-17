#ifndef _EASYTEXPACKER_RECT_BIN_ARRANGE_H_
#define _EASYTEXPACKER_RECT_BIN_ARRANGE_H_

#include "BaseStrategy.h"

struct RectSize;
struct Rect;

namespace etexpacker
{

class RectBinArrange : public BaseStrategy
{
public:
	virtual void Arrange(const std::vector<ee::ImageSprite*>& sprs);
	virtual int GetTextureAccount() const {
		return m_tex_account;
	}

private:
	void BeforePacking(const std::vector<ee::ImageSprite*>& sorted, 
		std::vector<RectSize>& input) const;
	void AfterPacking(float xoffset,
		std::vector<ee::ImageSprite*>& sprs, 
		const std::vector<Rect>& output, 
		std::vector<ee::ImageSprite*>& remains) const;

	void GuillotineBinPackAlg(const std::vector<RectSize>& input, std::vector<Rect>& output) const;
	void MaxRectsBinPackAlg(std::vector<RectSize>& input, std::vector<Rect>& output) const;
	void ShelfBinPackAlg(const std::vector<RectSize>& input, std::vector<Rect>& output) const;
	void SkylineBinPackAlg(std::vector<RectSize>& input, std::vector<Rect>& output) const;

private:
	int m_tex_account;

}; // RectBinArrange

}

#endif // _EASYTEXPACKER_RECT_BIN_ARRANGE_H_