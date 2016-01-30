#pragma once

#include <drag2d.h>

namespace edb
{
	struct Node
	{
		std::vector<d2d::Sprite*> out, in;
	};

	class Graphics
	{
	public:
		void connect(d2d::Sprite* from, d2d::Sprite* to);

		void move(d2d::Sprite* sprite, const d2d::Vector& offset);

		Node* query(d2d::Sprite* sprite) const;

		void clear();

	public:
		std::map<d2d::Sprite*, Node*> connection;
	};
}