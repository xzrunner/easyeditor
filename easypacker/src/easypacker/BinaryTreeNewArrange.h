#ifndef EPACKER_BINARY_TREE_NEW_ARRANGE_H
#define EPACKER_BINARY_TREE_NEW_ARRANGE_H

#include "BaseStrategy.h"

namespace epacker
{

class BinaryTreeNewArrange : public BaseStrategy
{
public:
	BinaryTreeNewArrange();
	virtual ~BinaryTreeNewArrange();

	virtual void arrange(const std::vector<d2d::ImageSprite*>& sprites);
	virtual int GetTextureAccount() const {
		return m_tex_account;
	}

private:
	void InitRoot();

	bool Insert(d2d::ImageSprite& img) const;

private:
	#define HASH_SIZE 7561
	#define NODE_SIZE 4096*2

private:
	d2d::TPNode* m_root;

	std::map<std::string, d2d::TPNode*> m_mapImages;

	int m_tex_account;

}; // BinaryTreeNewArrange

}

#endif // EPACKER_BINARY_TREE_NEW_ARRANGE_H