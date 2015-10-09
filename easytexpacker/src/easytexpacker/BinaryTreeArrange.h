#ifndef _EASYTEXPACKER_BINARY_TREE_ARRANGE_H_
#define _EASYTEXPACKER_BINARY_TREE_ARRANGE_H_

#include "BaseStrategy.h"

namespace etexpacker
{
	class BinaryTreeArrange : public BaseStrategy
	{
	public:
		BinaryTreeArrange();
		virtual ~BinaryTreeArrange();

		virtual void arrange(const std::vector<d2d::ImageSprite*>& sprites);
		virtual int GetTextureAccount() const { return 1; }

	private:
		void resetRoot();

	private:
		struct Node
		{
			Node* child[2];
			d2d::ISprite* texture;
			d2d::Rect rc;

			Node();
			~Node();

			Node* insert(d2d::ImageSprite* sprite, int flag);
		};

		static const int REINSERT_MAX = 128;
		static const int SPACING = 128;

	private:
		Node* m_root;

	}; // BinaryTreeArrange
}

#endif // _EASYTEXPACKER_BINARY_TREE_ARRANGE_H_