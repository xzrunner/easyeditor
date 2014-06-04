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

private:
	struct Node
	{
		const d2d::ImageSprite* image;

		int xmin, ymin, xmax, ymax;
		bool is_rotated;
		bool is_split_y;

		Node* next;
		Node* child;
		Node* parent;

		int remain_area;
		int remain_len;
		int remain_space;

	
	public:
		Node();
		Node(int width, int height);
		~Node() {
			// todo
		}

		Node* Insert(const d2d::ImageSprite& img, int w, int h);
	
		bool IsRoomEnough(int w, int h) const;

	private:
		Node* Split(int w, int h);

		float GetArea() const;
		float GetMaxLength() const;

		void UpdateRemain();
	};

private:
	void InitRoot();

	bool Insert(d2d::ImageSprite& img) const;

private:
	#define HASH_SIZE 7561
	#define NODE_SIZE 4096*2

private:
	Node* m_root;

	std::map<std::string, Node*> m_mapImages;

}; // BinaryTreeNewArrange

}

#endif // EPACKER_BINARY_TREE_NEW_ARRANGE_H