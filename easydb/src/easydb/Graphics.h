#pragma once

#include <drag2d.h>

namespace edb
{
	struct Node
	{
		std::vector<d2d::ISprite*> out, in;
	};

	class Graphics
	{
	public:
		void connect(d2d::ISprite* from, d2d::ISprite* to);

		void move(d2d::ISprite* sprite, const d2d::Vector& offset);

		Node* query(d2d::ISprite* sprite) const;

		void clear();

	public:
		std::map<d2d::ISprite*, Node*> connection;
	};
}