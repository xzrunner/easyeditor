#ifndef _EASYTEXPACKER_BINARY_TREE_NEW_ARRANGE_H_
#define _EASYTEXPACKER_BINARY_TREE_NEW_ARRANGE_H_

#include "BaseStrategy.h"

namespace etexpacker
{

class BinaryTreeNewArrange : public BaseStrategy
{
public:
	BinaryTreeNewArrange();
	virtual ~BinaryTreeNewArrange();

	virtual void arrange(const std::vector<ee::ImageSprite*>& sprites);
	virtual int GetTextureAccount() const {
		return m_tex_account;
	}

private:
	void InitRoot();

	bool Insert(ee::ImageSprite& img) const;

private:
	#define HASH_SIZE 7561
	#define NODE_SIZE 4096*2

private:
	ee::TPNode* m_root;

	std::map<std::string, ee::TPNode*> m_mapImages;

	int m_tex_account;

}; // BinaryTreeNewArrange

}

#endif // _EASYTEXPACKER_BINARY_TREE_NEW_ARRANGE_H_