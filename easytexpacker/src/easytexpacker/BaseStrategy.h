#ifndef _EASYTEXPACKER_BASE_STRATEGY_H_
#define _EASYTEXPACKER_BASE_STRATEGY_H_

#include <drag2d.h>

namespace etexpacker
{
	class BaseStrategy
	{
	public:
		virtual ~BaseStrategy() {}

		virtual void arrange(const std::vector<d2d::ImageSprite*>& sprites) = 0;
		virtual int GetTextureAccount() const = 0;

	protected:
		void sortByArea(std::vector<d2d::ImageSprite*>& sprites, bool isDescend = true) const;
		void sortByMaxEdge(std::vector<d2d::ImageSprite*>& sprites, bool isDescend = true) const;
		void sortByTotEdges(std::vector<d2d::ImageSprite*>& sprites, bool isDescend = true) const;

	protected:
		enum SortStrategy
		{
			e_area,
			e_maxEdge,
			e_totEdges
		};

		class SpriteCmp
		{
		public:
			SpriteCmp(SortStrategy strategy, bool isDescend);

			bool operator() (const d2d::ImageSprite* s0, const d2d::ImageSprite* s1) const;

		private:
			bool isAreaLess(const d2d::ImageSprite* s0, const d2d::ImageSprite* s1) const;
			bool isEdgeLess(const d2d::ImageSprite* s0, const d2d::ImageSprite* s1) const;
			bool isTotEdgesLess(const d2d::ImageSprite* s0, const d2d::ImageSprite* s1) const;

		private:
			SortStrategy m_strategy;
			bool m_isDescend;

		}; // SpriteCmp

	}; // BaseStrategy
}

#endif // _EASYTEXPACKER_BASE_STRATEGY_H_