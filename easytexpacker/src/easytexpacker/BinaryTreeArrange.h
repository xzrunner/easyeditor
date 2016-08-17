#ifndef _EASYTEXPACKER_BINARY_TREE_ARRANGE_H_
#define _EASYTEXPACKER_BINARY_TREE_ARRANGE_H_

#include "BaseStrategy.h"

namespace ee { class Sprite; }

namespace etexpacker
{

class BinaryTreeArrange : public BaseStrategy
{
public:
	BinaryTreeArrange();
	virtual ~BinaryTreeArrange();

	virtual void Arrange(const std::vector<ee::ImageSprite*>& sprs);
	virtual int GetTextureAccount() const { return 1; }

private:
	void ResetRoot();

private:
	struct Node
	{
		Node* child[2];
		ee::Sprite* texture;
		sm::rect rc;

		Node();
		~Node();

		Node* Insert(ee::ImageSprite* spr, int flag);
	};

	static const int REINSERT_MAX = 128;
	static const int SPACING = 128;

private:
	Node* m_root;

}; // BinaryTreeArrange

}

#endif // _EASYTEXPACKER_BINARY_TREE_ARRANGE_H_